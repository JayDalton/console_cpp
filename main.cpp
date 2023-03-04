#include "console.h"

/*
cmake -A Win32 -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:\Packages\scripts\buildsystems\vcpkg.cmake
-DCMAKE_BUILD_TYPE=DEBUG -G "Visual Studio 17 2022" 
&& cmake --build .\build\ --config Release
*/

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp> 
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

auto main(int argc, char const * const argv[]) -> int
{
   const cv::String keys = 
      "{help h usage ? |      | print this message   }"
      "{@source        |<none>| source for process   }"
      "{@target        |      | target for process   }"
      // "{@repeat        |1     | number               }"
      // "{fps            | -1.0 | fps for output video }"
      // "{N count        |100   | count of objects     }"
   ;

   cv::CommandLineParser parser(argc, argv, keys);
   parser.about("Console cpp v1.0.0");

   if (parser.has("help"))
   {
      parser.printMessage();
      return 1;
   }

   if (!parser.check())
   {
      parser.printErrors();
      return 1;
   }

   String source = parser.get<String>(0);
   String target = parser.get<String>(1);
   // int repeat = parser.get<int>(2);
   // double fps = parser.get<double>("fps");
   // int N = parser.get<int>("N");

   cmd::Console console;
   return console.run(source);
}

