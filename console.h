#pragma once

#include "common.h"

// #include "windows.h"
#include "dicom.h"
#include "matrix.h"

namespace cmd {

   enum class Category { None, Menu, List, Info, Show };

   using ResultList = std::vector<Category>;

   //////////////////////////////////////////////////////////////////////////////

   struct Repository
   {
      auto isStopword(const String& word) const -> bool
      {
         return m_stopwords.contains(word);
      }

      auto findRoot(const String& word) const -> String
      {
         if (m_rootword.contains(word))
         {
            return m_rootword.at(word);
         }
         return word;
      }

      auto getIntent(const String& word) const -> std::optional<Category>
      {
         if (m_meanings.contains(word))
         {
            return m_meanings.at(word);
         }
         return std::nullopt;
      }

   private:
      std::unordered_set<String> m_stopwords{{
         "me", "an", "a", "in"
      }};

      std::map<String, String> m_rootword{{
         // {"home", "menu"}, ???
         {"view", "show"},
      }};

      std::map<String, Category> m_meanings{{
         {"menu", Category::Menu},
         {"list", Category::List},
         {"info", Category::Info},
         {"show", Category::Show},
      }};
   };

   auto normalize(StringView input) -> StringList
   {
      using namespace std::views;

      auto words = input 
         | filter([](unsigned char c) { return std::isalpha(c) || std::isspace(c); })
         | transform([](unsigned char c) { return std::tolower(c); })
         // | filter/remove consecutive spaces, C++23?
         | split(' ')
         | transform([](auto&& c) -> String { return { c.begin(), c.end() }; })
         | filter([](auto&& c) { return !c.empty(); })
      ;

      return { words.begin(), words.end() };
   }

   //////////////////////////////////////////////////////////////////////////////

   struct Recognizer
   {
      auto calculate(StringView input) -> ResultList
      {
         return exctract(normalize(input));
      }

      auto exctract(const StringList& words) -> ResultList
      {
         using namespace std::views;

         auto intent = words
            | filter([this](const String& word) { return !m_repo.isStopword(word); })
            | transform([this](const String& word) { return m_repo.findRoot(word); })
            | transform([this](const String& word) { return m_repo.getIntent(word); })
            | filter([](auto&& opt){ return opt.has_value(); })
            | transform([](auto&& opt){ return opt.value(); })
         ;

         return { intent.begin(), intent.end() };
      }

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

   auto get_file_list(std::string_view source) -> PathList
   {
      using namespace std::ranges::views;

      if (!fs::exists(source) || !fs::is_directory(fs::status(source)))
      {
         print("source is no directory: {}\n", source);
         return {};
      }

      return fs::directory_iterator{source}
         | filter([](auto&& entry){ return entry.exists(); })
         | filter([](auto&& entry){ return entry.is_regular_file(); })
         | filter([](auto&& entry){ return 0 < entry.file_size(); })
         | transform([](auto&& entry){ return entry.path(); })
         | filter([](auto&& path){ return path.extension() == ".dcm";})
         | std::ranges::to<PathList>()
      ;
   }

   auto show_list(std::string_view folder) -> void
   {
      auto res = get_file_list(folder) 
         | std::views::transform([](auto&& path){ return dcm::read_file_info(path); })
         | std::views::transform([](auto&& info){ return dcm::format_file_info(info); })
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

   auto interpret(StringOpt&& input) -> void
   {
      
   }

   struct Console
   {
      Console() { print("-- ctor\n"); }

      int run(std::string_view folder)
      {
         print("Contemporary C++ in Action\n");

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

