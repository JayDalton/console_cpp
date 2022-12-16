cmake_minimum_required(VERSION 3.24)

project(console_cpp VERSION 0.1) 

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(CMAKE_GENERATOR_PLATFORM "Win32")
# set(CMAKE_TOOLCHAIN_FILE "C:/Packages/scripts/buildsystems/vcpkg.cmake")

find_package( OpenCV REQUIRED )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# target_link_libraries(${PROJECT_NAME}
#   PRIVATE
#     asio::asio
#     PkgConfig::LIBAV
#     SDL2::SDL2 SDL2::SDL2main 
# )

target_sources(${PROJECT_NAME} PRIVATE
  "c_ressource.h"
  "common.h"
  "main.cpp"
)


# # Set OpenCVConfig.cmake Search Directory
# set( OpenCV_DIR )
# if( NOT CMAKE_CL_64 )
#   set( OpenCV_DIR "C:/Packages/installed/x86-windows/share/opencv" )
# else()
#   set( OpenCV_DIR "C:/Packages/installed/x64-windows/share/opencv" )
# endif()

# # Find Package
# find_package( OpenCV REQUIRED )

# if( OpenCV_FOUND )
#   # Additional Include Directories
#   include_directories( ${OpenCV_INCLUDE_DIRS} )

#   # Additional Library Directories
#   link_directories( ${OpenCV_LIB_DIR} )

#   # Additional Dependencies
#   target_link_libraries(console_cpp ${OpenCV_LIBS})
# endif()
