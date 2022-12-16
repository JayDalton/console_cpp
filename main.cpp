#include "common.h"

#include "dicom.h"
#include "matrix.h"

/*
cmake -A Win32 -B build -S . -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_TOOLCHAIN_FILE=C:\Packages\scripts\buildsystems\vcpkg.cmake
&& cmake --build .\build\ --config Release
&& cmake --build .\build\
*/

// #include <dcmtk/config/osconfig.h>    /* make sure OS specific configuration is included first */
// #include <dcmtk/dcmdata/dcfilefo.h>
// #include <dcmtk/dcmdata/dcmetinf.h>
// #include <dcmtk/dcmimgle/dcmimage.h>

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
      return 0;
   }

   if (!parser.check())
   {
      parser.printErrors();
      return 0;
   }

   String sourceFile = parser.get<String>(0);
   String targetFile = parser.get<String>(1);
   // int repeat = parser.get<int>(2);
   // double fps = parser.get<double>("fps");
   // int N = parser.get<int>("N");

   if (!fs::exists(sourceFile))
   {
      print("file does not exists: {}", sourceFile);
      return 1;
   }
   
   print("Source File is: {}\n", sourceFile);

   // DcmFileFormat format;
   // DcmDataset dataset;

   cv::Mat img = cv::imread(sourceFile, cv::IMREAD_COLOR);

   if (img.empty())
   {
      print("Could not read the image: {}", sourceFile);
      return 1;
   }

   print("...\n");

   cv::imshow("Display window", img);

   int k = cv::waitKey(0); // Wait for a keystroke in the window
   // if (k == 's')
   // {
   //     cv::imwrite("starry_night.png", img);
   // }

   return {};
}

