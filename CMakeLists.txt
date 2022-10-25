cmake_minimum_required(VERSION 3.18)

project(console_cpp LANGUAGES CXX VERSION 0.1) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_FILES 
    common.h
    main.cpp
)

add_executable(console_cpp ${SOURCE_FILES})
