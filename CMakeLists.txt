cmake_minimum_required(VERSION 3.24)

project(console_cpp VERSION 0.1) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if( MSVC )
  message("MSVC found") # needed by dcmtk
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20 /Zc:__cplusplus")
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Set OpenCVConfig.cmake Search Directory
set( OpenCV_DIR )
if (NOT CMAKE_CL_64 )
  set( OpenCV_DIR "C:/Packages/installed/x86-windows/share/opencv" )
else()  
  set( OpenCV_DIR "C:/Packages/installed/x64-windows/share/opencv" )
endif()  

find_package(asio CONFIG REQUIRED)
find_package(DCMTK CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)

if( OpenCV_FOUND )
  message("OpenCV found")
  include_directories(${OpenCV_INCLUDE_DIRS})
  link_directories(${OpenCV_LIB_DIR})
  # target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
endif()

if( DCMTK_FOUND )
  message("DCMTK found")
  include_directories(${DCMTK_INCLUDE_DIRS})
  link_directories(${DCMTK_LIBRARIES})
  # target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
endif()

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_SOURCE_DIR}/assets 
      ${CMAKE_SOURCE_DIR})

# add_custom_command(
#   TARGET ${PROJECT_NAME} POST_BUILD
#   COMMENT "copying dcm-file"
#   COMMAND ${CMAKE_COMMAND} -E copy
#      ${CMAKE_SOURCE_DIR}/assets/PE_Image.jpg
#      ${CMAKE_SOURCE_DIR}/PE_Image.jpg)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    asio::asio
    # DCMTK::cmr 
    # DCMTK::i2d 
    # DCMTK::ijg8 
    DCMTK::DCMTK
    ${OpenCV_LIBS}
    # opencv_core 
    # opencv_highgui 
    # opencv_imgproc 
    # opencv_imgcodecs 
    # opencv_videoio 
)

target_sources(${PROJECT_NAME} PRIVATE
  "c_ressource.h"
  "common.h"
  "dicom.h"
  "main.cpp"
  "matrix.h"
)

