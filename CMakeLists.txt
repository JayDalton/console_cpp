cmake_minimum_required(VERSION 3.24)

project(console_cpp VERSION 0.1) 

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# Set OpenCVConfig.cmake Search Directory
set(OpenCV_DIR )
if (NOT CMAKE_CL_64 )
  set( OpenCV_DIR "C:/Packages/installed/x86-windows/share/opencv" )
else()  
  set( OpenCV_DIR "C:/Packages/installed/x64-windows/share/opencv" )
endif()  

find_package(asio REQUIRED)
find_package(DCMTK REQUIRED)
find_package(OpenCV REQUIRED)

if( OpenCV_FOUND )
  # Additional Include Directories
  include_directories(${OpenCV_INCLUDE_DIRS})

  # Additional Library Directories
  link_directories(${OpenCV_LIB_DIR})

  # Additional Dependencies
  # target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
endif()

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMENT "copying dcm-file"
  COMMAND ${CMAKE_COMMAND} -E copy
     ${CMAKE_SOURCE_DIR}/assets/PE_Image.dcm
     ${CMAKE_BINARY_DIR}/${CONFIG}/PE_Image.dcm)

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
  "main.cpp"
)

