#include "common.h"

// #include "dicom.h"
// #include "matrix.h"

/*
cmake -A Win32 -B build -S . -DCMAKE_BUILD_TYPE=DEBUG -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=C:\Packages\scripts\buildsystems\vcpkg.cmake
&& cmake --build .\build\ --config Release
&& cmake --build .\build\
*/

#include <dcmtk/config/osconfig.h>    /* make sure OS specific configuration is included first */
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcmetinf.h>
#include <dcmtk/dcmimgle/dcmimage.h>


#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp> 
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

auto main(int argc, char const * const argv[]) -> int
{
   print("... {} ...\n", __cplusplus);

   auto const ints = {0,1,2,3,4,5};
   auto even = [](int i) { return 0 == i % 2; };
   auto square = [](int i) { return i * i; };

    // "pipe" syntax of composing the views:
    for (int i : ints | std::views::filter(even) | std::views::transform(square)) {
        std::cout << i << ' ';
    }

    std::cout << '\n';

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

   if (fs::path(sourceFile).extension() == ".dcm")
   {
      print("DCM file detected\n");
      // DicomImage image(sourceFile.c_str());

      // auto frameCount = image.getFrameCount();
      // auto firstFrame = image.getFirstFrame();
      // auto frameTime = image.getFrameTime();

      // auto numOfFrames = image.getNumberOfFrames();
      // auto repoFrame = image.getRepresentativeFrame();
      // auto dataSize = image.getOutputDataSize();

      // auto d = image.getDepth();
      // auto h = image.getHeight();
      // auto w = image.getWidth();

      // print("image: "
      //    "d {}, h {}, w {}\n"
      //    "fc {}, ff {}, ft {}\n"
      //    "nf {}, rf {}, ds {}\n"
      //    "\n", d, h, w
      // );
   }

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

