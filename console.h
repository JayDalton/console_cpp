#pragma once

#include "common.h"

struct Console
{
   int run(std::string_view folder)
   {
      print("Contemporary C++ in Action\n");

      for (auto&& path : get_file_list(folder))
      {
         print("{} \n", path.filename().string());

      }

      show_menu();

      while (auto input = readInput())
      {
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

private:
};
