cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

project(spbachelor VERSION 0.1.0 LANGUAGES CXX)

#Path of CImg.h file relative to this file path
set(CIMG_H_PATH ${PROJECT_SOURCE_DIR}/vendor/CImg)

set(CMAKE_FILE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(APPLE)
   include_directories(AFTER "/opt/X11/include")
endif()

include("${CMAKE_FILE_PATH}/FindX11.cmake")
include("${CMAKE_FILE_PATH}/FindLibJPEG.cmake")
include("${CMAKE_FILE_PATH}/FindLibPNG.cmake")

add_subdirectory(src)
