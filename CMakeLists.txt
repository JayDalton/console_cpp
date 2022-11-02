cmake_minimum_required(VERSION 3.24)

project(console_cpp VERSION 0.1) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_GENERATOR_PLATFORM "Win32")
set(CMAKE_TOOLCHAIN_FILE "C:/Packages/scripts/buildsystems/vcpkg.cmake")

set(SOURCE_FILES 
    common.h
    main.cpp
)

add_executable(console_cpp ${SOURCE_FILES})

# Set OpenCVConfig.cmake Search Directory
set( OpenCV_DIR )
if( NOT CMAKE_CL_64 )
  set( OpenCV_DIR "C:/Packages/installed/x86-windows/share/opencv" )
else()
  set( OpenCV_DIR "C:/Packages/installed/x64-windows/share/opencv" )
endif()

# Find Package
find_package( OpenCV REQUIRED )

if( OpenCV_FOUND )
  # Additional Include Directories
  include_directories( ${OpenCV_INCLUDE_DIRS} )

  # Additional Library Directories
  link_directories( ${OpenCV_LIB_DIR} )

  # Additional Dependencies
  target_link_libraries(console_cpp ${OpenCV_LIBS})
endif()
