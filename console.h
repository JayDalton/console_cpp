#pragma once

#include "common.h"

// #include "windows.h"
#include "dicom.h"
#include "matrix.h"

namespace cmd {

   enum class Category { None, Menu, List, Info, Show };

   using ResultList = std::vector<Category>;

   using CmdParam = std::variant<std::monostate, int, float, Category>;



   //////////////////////////////////////////////////////////////////////////////

   struct Repository
   {
      auto getIntent(StringView view) const -> std::optional<Category>
      {
         String word{view.begin(), view.end()};
         if (m_meanings.contains(word))
         {
            return m_meanings.at(word);
         }
         return std::nullopt;
      }

   private:
      std::map<String, Category> m_meanings{{
         {"menu", Category::Menu},
         {"home", Category::Menu},
         {"list", Category::List},
         {"info", Category::Info},
         {"show", Category::Show},
         {"view", Category::Show},
      }};
   };

   //////////////////////////////////////////////////////////////////////////////

   auto normalize(StringView input) -> StringList
   {
      using namespace std::views;

      // list
      // info 2
      // show 1

      auto words = input 
         | transform([](unsigned char c) { return std::tolower(c); })
         // | filter/remove consecutive spaces, C++23?         
         | split(' ')
         | transform([](auto&& c) -> String { return { c.begin(), c.end() }; })
         | filter([](auto&& c) { return !c.empty(); })
      ;

      return { words.begin(), words.end() };
   }

   struct Recognizer
   {
      auto to_int(StringView sv) const -> Optional<int>
      {
         int r{};
         auto [ptr, ec] { std::from_chars(sv.data(), sv.data() + sv.size(), r) };
         if (ec == std::errc()) { return r; } else { return std::nullopt; }
      }

      auto to_float(StringView sv) const -> Optional<float>
      {
         float r{};
         auto [ptr, ec] { std::from_chars(sv.data(), sv.data() + sv.size(), r) };
         if (ec == std::errc()) { return r; } else { return std::nullopt; }
      }

      auto to_value(StringView input) const -> CmdParam
      {
         if (auto value = to_float(input))
         {
            return value.value();
         }
         else if (auto value = to_int(input))
         {
            return value.value();
         }
         else if (auto value { m_repo.getIntent(input)})
         {
            return value.value();
         }
         return {};
      }

      auto interpret(StringView input) -> StringList
      {
         using namespace std::views;

         auto words = input 
            | transform([](unsigned char c) { return std::tolower(c); })
            | split(' ')
            | transform([](auto&& c) -> String { return { c.begin(), c.end() }; })
            | filter([](auto&& c) { return !c.empty(); })
            // | transform([this](String&& str){ return to_value(str); })
         ;

         return { words.begin(), words.end() };
      }

      // auto exctract(const StringList& words) -> ResultList
      // {
      //    using namespace std::views;

      //    auto intent = words
      //       | filter([this](const String& word) { return !m_repo.isStopword(word); })
      //       | transform([this](const String& word) { return m_repo.findRoot(word); })
      //       | transform([this](const String& word) { return m_repo.getIntent(word); })
      //       | filter([](auto&& opt){ return opt.has_value(); })
      //       | transform([](auto&& opt){ return opt.value(); })
      //    ;

      //    return { intent.begin(), intent.end() };
      // }

   private:
      Repository m_repo;
   };


   auto createCommand() 
   {
      return [](){

      };
   }

   auto show_menu() -> void
   {
      print("Commands: \n"
         "menu - prints list of commands \n"
         "list - list all found dcm files \n"
         "info - info of indexed dcm file \n"
         "show - show indexed dcm file \n"
      );
   }

   auto show_list(std::string_view folder) -> void
   {
      auto res = dcm::get_file_list(folder) 
         | std::views::transform(dcm::read_file_info)
         | std::views::transform(dcm::format_file_info)
      ;

      std::ranges::copy(res, std::ostream_iterator<String>(std::cout, "\n"));
   }

   StringOpt readInput()
   {
       while (true)
       {
           try
           {
               String input;
               print("Input: ");
               std::getline(std::cin, input);
               if (!input.empty()) {
                  return input;
               }
               return std::nullopt;
           }
           catch (...) {}
       }
   }

   struct Console
   {
      int run(std::string_view folder)
      {
         print("Dicom Image Info\n");

         // show_menu();
         show_list(folder);

         while (auto input = readInput())
         {
            // for (auto item : m_engine.calculate(input.value()))
            // {
            //    print("i ");
            // }

         }

         print("End!\n");
         return {};
      }

   private:
      Recognizer m_engine;
   };

}

