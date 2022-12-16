#include "common.h"

/*
cmake -A Win32 -B build -S . -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_TOOLCHAIN_FILE=C:\Packages\scripts\buildsystems\vcpkg.cmake
&& cmake --build .\build\ --config Release
&& cmake --build .\build\
*/

// mkdir build && cd build && cmake -A Win32 .. && cmake --build . --config Release

// cmake -G "Visual Studio 16 2019" -A Win32 -S \path_to_source\ -B "build32"
// cmake -G "Visual Studio 16 2019" -A x64 -S \path_to_source\ -B "build64"
// cmake --build build32 --config Release
// cmake --build build64 --config Release

// #include <dcmtk/config/osconfig.h>    /* make sure OS specific configuration is included first */
// #include <dcmtk/dcmdata/dcfilefo.h>
// #include <dcmtk/dcmdata/dcmetinf.h>
// #include <dcmtk/dcmimgle/dcmimage.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


auto main(int argc, char const * const argv[]) -> int
{
   const cv::String keys = 
      "{help h usage ? |      | print this message   }"
      // "{@image1        |      | image1 for compare   }"
      "{@image2        |<none>| image2 for compare   }"
      // "{@repeat        |1     | number               }"
      // "{path           |.     | path to file         }"
      // "{fps            | -1.0 | fps for output video }"
      // "{N count        |100   | count of objects     }"
      // "{ts timestamp   |      | use time stamp       }"
   ;

   cv::CommandLineParser parser(argc, argv, keys);
   parser.about("Application name v1.0.0");

   if (parser.has("help"))
   {
      parser.printMessage();
      return 0;
   }

   // int N = parser.get<int>("N");
   // double fps = parser.get<double>("fps");
   // String path = parser.get<String>("path");
   // auto use_time_stamp = parser.has("timestamp");
   String img1 = parser.get<String>(0);
   // String img2 = parser.get<String>(1);
   // int repeat = parser.get<int>(2);

   if (!parser.check())
   {
      parser.printErrors();
      return 0;
   }

   print("Image: {}\n", img1);

   // DcmFileFormat format;
   // DcmDataset dataset;

   // auto const args{std::span{argv, argv + argc}};

   // if (auto res = acme::test(args))
   // {
   //     print("{}\n", res.value());
   // }

   std::string image_path = "PE_Image.jpg";
   std::ifstream file(image_path);
   if (file.is_open())
   {
      print("is open\n");
   }
   else 
   {
      print("is not\n");
   }


   cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);

   if (img.empty())
   {
      print("Could not read the image: {}", image_path);
      return 1;
   }

   print("...\n");

   cv::imshow("Display window", img);

   int k = cv::waitKey(0); // Wait for a keystroke in the window
   // if (k == 's')
   // {
   //     cv::imwrite("starry_night.png", img);
   // }

   return 0;
}

