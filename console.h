#pragma once

#include "common.h"

namespace cmd {

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

   auto get_file_list(std::string_view folder) -> PathList
   {
      using namespace std::ranges::views;

      return fs::directory_iterator{folder}
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
         | std::views::transform([](auto&& path){ return path.filename().string(); });

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
      int run(std::string_view folder)
      {
         print("Contemporary C++ in Action\n");

         show_menu();
         show_list(folder);

         while (auto input = readInput())
         {
            // auto [x,y] = m_engine.find(input.value());
            // if () 
            // {

            // }

            for (auto c : (*input))
            {
               // print("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}\n", c);
               print(L"int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}\n", c);
            }
         }

         print("End!\n");
         return {};
      }

   private:
      // engine
   };

}

