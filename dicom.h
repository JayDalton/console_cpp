#pragma once

#include "common.h"

#include <dcmtk/config/osconfig.h>    /* make sure OS specific configuration is included first */
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcmetinf.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcistrmf.h> 
#include <dcmtk/dcmdata/dcpxitem.h>

namespace dcm {


   // print("directory_iterator:\n");
   // using namespace std::ranges::views;

   // static std::string_view source{"..."};

   // if (fs::path(source).extension() == ".dcm")
   // {
   //    print("DCM file detected\n");


   //    /* approach 1 */
   //    {
   //       OFCondition status;
   //       DcmMetaInfo metainfo;
   //       status = metainfo.loadFile(source.c_str());
   //       if (status.good())
   //       {
   //          OFString sopClassUID, xferUID;
   //          if (metainfo.findAndGetOFString(DCM_MediaStorageSOPClassUID, sopClassUID).good())
   //                std::cout << "SOP Class UID: " << sopClassUID << OFendl;
   //          if (metainfo.findAndGetOFString(DCM_TransferSyntaxUID, xferUID).good())
   //                std::cout << "Transfer Syntax UID: " << xferUID << OFendl;
   //          metainfo.print(std::cout);
   //       }
   //    }

   //    print("----------------- 1\n");

   //    {

   //       print("FileFormat\n");
   //       OFCondition result = EC_Normal;
   //       /* Load file and get pixel data element */
   //       DcmFileFormat dfile;
   //       result = dfile.loadFile(source.c_str());
   //       if (result.bad())
   //       {
   //          print("load file\n");
   //          return 1;
   //       }
   //       DcmDataset *data = dfile.getDataset();
   //       if (data == NULL)
   //       {
   //          print("dataset\n");
   //          return 1;
   //       }
   //       DcmElement* element = NULL;
   //       result = data->findAndGetElement(DCM_PixelData, element);
   //       if (result.bad() || element == NULL)
   //       {
   //          print("GetElement\n");
   //          return 1;
   //       }
   //       DcmPixelData *dpix = NULL;
   //       dpix = OFstatic_cast(DcmPixelData*, element);

   //       /* Since we have compressed data, we must utilize DcmPixelSequence
   //          in order to access it in raw format, e. g. for decompressing it
   //          with an external library.
   //          */
   //       DcmPixelSequence *dseq = NULL;
   //       E_TransferSyntax xferSyntax = EXS_Unknown;
   //       const DcmRepresentationParameter *rep = NULL;
   //       // Find the key that is needed to access the right representation of the data within DCMTK
   //       dpix->getOriginalRepresentationKey(xferSyntax, rep);
   //       // Access original data representation and get result within pixel sequence
   //       result = dpix->getEncapsulatedRepresentation(xferSyntax, rep, dseq);
   //       print("getEncapsulatedRepresentation\n");
   //       if ( result == EC_Normal )
   //       {
   //          print("Encapsulated Representation\n");
   //          DcmPixelItem* pixitem = NULL;
   //          // Access first frame (skipping offset table)
   //          dseq->getItem(pixitem, 1);
   //          if (pixitem == NULL)
   //          {
   //             print("getItem\n");
   //             return 1;
   //          }
   //          Uint8* pixData = NULL;
   //          // Get the length of this pixel item (i.e. fragment, i.e. most of the time, the lenght of the frame)
   //          Uint32 length = pixitem->getLength();
   //          if (length == 0)
   //          {
   //             print("getLength\n");
   //             return 1;
   //          }
   //          // Finally, get the compressed data for this pixel item
   //          result = pixitem->getUint8Array(pixData);
   //          // Do something useful with pixData...
   //       }
   //       else
   //          print("result bad\n");
   //    }

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

   struct FileInfo 
   {
      double FrameTime{0};
      std::uint64_t FrameCount{0};
      std::uint64_t FirstFrame{0};
      std::uint64_t DataSize{0};
      std::uint64_t Height{0};
      std::uint64_t Width{0};
      std::uint32_t Depth{0};
      bool isMonochrome{false};
      fs::path Path;
   };

   auto read_file_info(const fs::path& path) -> FileInfo 
   {
      auto source = path.string();
      DicomImage image(source.c_str());

      if (image.getStatus() != EI_Status::EIS_Normal)
      {
         return {};
      }

      FileInfo info;
      info.FrameCount = image.getFrameCount();
      info.FirstFrame = image.getFirstFrame();
      info.FrameTime = image.getFrameTime();

      // info.numOfFrames = image.getNumberOfFrames();
      // info.repoFrame = image.getRepresentativeFrame();
      info.DataSize = image.getOutputDataSize();

      info.Depth = image.getDepth();
      info.Height = image.getHeight();
      info.Width = image.getWidth();
      info.Path = path;

      info.isMonochrome = image.isMonochrome();

      return info;
   }

   auto format_file_info(const FileInfo& info) -> String 
   {
      return std::format("{:20} | {}x{} | {:3} {:5} {:9} | {}: {} {}"
         , info.Path.filename().string()
         , info.Height
         , info.Width
         , info.isMonochrome ? "mono" : "color"
         , info.Depth
         , info.DataSize
         , info.FrameCount
         , info.FirstFrame
         , info.FrameTime
      );
   }


   //    print("----------------- 2\n");

   //    {
   //       DicomImage image(source.c_str());

   //       auto frameCount = image.getFrameCount();
   //       auto firstFrame = image.getFirstFrame();
   //       auto frameTime = image.getFrameTime();

   //       auto numOfFrames = image.getNumberOfFrames();
   //       auto repoFrame = image.getRepresentativeFrame();
   //       auto dataSize = image.getOutputDataSize();

   //       auto d = image.getDepth();
   //       auto h = image.getHeight();
   //       auto w = image.getWidth();

   //       print("image: "
   //          "d {}, h {}, w {}\n"
   //          "fc {}, ff {}, ft {}\n"
   //          "nf {}, rf {}, ds {}\n"
   //          "\n", d, h, w, 
   //          frameCount, firstFrame, frameTime,
   //          numOfFrames, repoFrame, dataSize         
   //       );

   //       const int m_depth{8};
   //       const auto status{image.getStatus()};
   //       if (status != EIS_Normal) { print("dicom image status\n"); }

   //       const std::unordered_map<int, std::pair<int, int>> formats {
   //          {8, {CV_8UC1, CV_8UC3}},
   //          {16, {CV_16UC1, CV_16UC3}},
   //       };

   //       auto isMono {image.isMonochrome()};
   //       auto format {formats.at(m_depth)};
   //       auto type {isMono ? format.first : format.second};
   //       auto dims {isMono ? 1 : 3};

   //       auto width {static_cast<int>(image.getWidth())};
   //       auto height {static_cast<int>(image.getHeight())};
   //       auto length {static_cast<int>(image.getOutputDataSize(m_depth))};
   //       auto buffer {image.getOutputData(m_depth, 0)}; // immer nur ein Frame drin!
   //       // dpassert(buffer && (length == width * height * dims));

   //       auto matrix {cv::Mat(height, width, type, const_cast<void*>(buffer)).clone()}; // copy

   //       print("matrix: "
   //          "chans {}, depth {}\n"
   //          "cols {}, rows {}\n"
   //          "dims {} flags {}\n",
   //          // "step {}, size {}\n",
   //          matrix.channels(), matrix.depth(),
   //          matrix.cols, matrix.rows, 
   //          matrix.dims, matrix.flags
   //          // matrix.step, matrix.size
   //       );

   //       print("...\n");
   //       cv::imshow("Display window", matrix);
   //       int k = cv::waitKey(0); // Wait for a keystroke in the window
   //    }
   // }
   
}