#include "common.h"

// #include "windows.h"
// #include "dicom.h"
// #include "matrix.h"

/*
cmake -A Win32 -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:\Packages\scripts\buildsystems\vcpkg.cmake
-DCMAKE_BUILD_TYPE=DEBUG -G "Visual Studio 17 2022" 
&& cmake --build .\build\ --config Release
*/

#include <dcmtk/config/osconfig.h>    /* make sure OS specific configuration is included first */
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcmetinf.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcistrmf.h> 
#include <dcmtk/dcmdata/dcpxitem.h>

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


      /* approach 1 */
      {
         OFCondition status;
         DcmMetaInfo metainfo;
         status = metainfo.loadFile(sourceFile.c_str());
         if (status.good())
         {
            OFString sopClassUID, xferUID;
            if (metainfo.findAndGetOFString(DCM_MediaStorageSOPClassUID, sopClassUID).good())
                  std::cout << "SOP Class UID: " << sopClassUID << OFendl;
            if (metainfo.findAndGetOFString(DCM_TransferSyntaxUID, xferUID).good())
                  std::cout << "Transfer Syntax UID: " << xferUID << OFendl;
            metainfo.print(std::cout);
         }
      }

      print("----------------- 1\n");

      {

         print("FileFormat\n");
         OFCondition result = EC_Normal;
         /* Load file and get pixel data element */
         DcmFileFormat dfile;
         result = dfile.loadFile(sourceFile.c_str());
         if (result.bad())
         {
            print("load file\n");
            return 1;
         }
         DcmDataset *data = dfile.getDataset();
         if (data == NULL)
         {
            print("dataset\n");
            return 1;
         }
         DcmElement* element = NULL;
         result = data->findAndGetElement(DCM_PixelData, element);
         if (result.bad() || element == NULL)
         {
            print("GetElement\n");
            return 1;
         }
         DcmPixelData *dpix = NULL;
         dpix = OFstatic_cast(DcmPixelData*, element);

         /* Since we have compressed data, we must utilize DcmPixelSequence
            in order to access it in raw format, e. g. for decompressing it
            with an external library.
            */
         DcmPixelSequence *dseq = NULL;
         E_TransferSyntax xferSyntax = EXS_Unknown;
         const DcmRepresentationParameter *rep = NULL;
         // Find the key that is needed to access the right representation of the data within DCMTK
         dpix->getOriginalRepresentationKey(xferSyntax, rep);
         // Access original data representation and get result within pixel sequence
         result = dpix->getEncapsulatedRepresentation(xferSyntax, rep, dseq);
         print("getEncapsulatedRepresentation\n");
         if ( result == EC_Normal )
         {
            print("Encapsulated Representation\n");
            DcmPixelItem* pixitem = NULL;
            // Access first frame (skipping offset table)
            dseq->getItem(pixitem, 1);
            if (pixitem == NULL)
            {
               print("getItem\n");
               return 1;
            }
            Uint8* pixData = NULL;
            // Get the length of this pixel item (i.e. fragment, i.e. most of the time, the lenght of the frame)
            Uint32 length = pixitem->getLength();
            if (length == 0)
            {
               print("getLength\n");
               return 1;
            }
            // Finally, get the compressed data for this pixel item
            result = pixitem->getUint8Array(pixData);
            // Do something useful with pixData...
         }
         else
            print("result bad\n");
      }


      print("----------------- 2\n");

      {
         DicomImage image(sourceFile.c_str());

         auto frameCount = image.getFrameCount();
         auto firstFrame = image.getFirstFrame();
         auto frameTime = image.getFrameTime();

         auto numOfFrames = image.getNumberOfFrames();
         auto repoFrame = image.getRepresentativeFrame();
         auto dataSize = image.getOutputDataSize();

         auto d = image.getDepth();
         auto h = image.getHeight();
         auto w = image.getWidth();

         print("image: "
            "d {}, h {}, w {}\n"
            "fc {}, ff {}, ft {}\n"
            "nf {}, rf {}, ds {}\n"
            "\n", d, h, w, 
            frameCount, firstFrame, frameTime,
            numOfFrames, repoFrame, dataSize         
         );

         const int m_depth{8};
         const auto status{image.getStatus()};
         if (status != EIS_Normal) { print("dicom image status\n"); }

         const std::unordered_map<int, std::pair<int, int>> formats {
            {8, {CV_8UC1, CV_8UC3}},
            {16, {CV_16UC1, CV_16UC3}},
         };

         auto isMono {image.isMonochrome()};
         auto format {formats.at(m_depth)};
         auto type {isMono ? format.first : format.second};
         auto dims {isMono ? 1 : 3};

         auto width {static_cast<int>(image.getWidth())};
         auto height {static_cast<int>(image.getHeight())};
         auto length {static_cast<int>(image.getOutputDataSize(m_depth))};
         auto buffer {image.getOutputData(m_depth, 0)}; // immer nur ein Frame drin!
         // dpassert(buffer && (length == width * height * dims));

         auto matrix {cv::Mat(height, width, type, const_cast<void*>(buffer)).clone()}; // copy

         print("matrix: "
            "chans {}, depth {}\n"
            "cols {}, rows {}\n"
            "dims {} flags {}\n",
            // "step {}, size {}\n",
            matrix.channels(), matrix.depth(),
            matrix.cols, matrix.rows, 
            matrix.dims, matrix.flags
            // matrix.step, matrix.size
         );

         print("...\n");
         cv::imshow("Display window", matrix);
         int k = cv::waitKey(0); // Wait for a keystroke in the window
      }
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

