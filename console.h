#pragma once

#include "common.h"

struct Console
{
   int run()
   {
      print("Contemporary C++ in Action\n");

      while (auto input = readWInput())
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

   std::optional<std::wstring> readWInput()
   {
       while (true)
       {
           try
           {
               // if constexpr w/string ?

               print("Input: ");
               std::wstring input;
               std::getline(std::wcin, input);
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
