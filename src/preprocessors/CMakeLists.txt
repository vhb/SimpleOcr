cmake_minimum_required(VERSION 2.6)

project(ocr_test_lib)
include_directories(..)

set(
    HUETHRESHOLD_SOURCE
    ./HueThreshold.cpp
    )
set(
    SMOOTH_SOURCE
    ./Smooth.cpp
    )

set(
    MORPHOLOGIC_FILTER_SOURCE
    ./MorphologicFilter.cpp
    )

set(
    GRAY_IMG
    ./GrayImg.cpp
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g3")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../..)

add_library(huethreshold      SHARED ${HUETHRESHOLD_SOURCE})
add_library(smooth            SHARED ${SMOOTH_SOURCE})
add_library(morphologicfilter SHARED ${MORPHOLOGIC_FILTER_SOURCE})
add_library(grayimg           SHARED ${GRAY_IMG})

target_link_libraries(huethreshold ${OpenCV_LIBS} image)
target_link_libraries(smooth ${OpenCV_LIBS} image)
target_link_libraries(morphologicfilter ${OpenCV_LIBS} image)
target_link_libraries(grayimg ${OpenCV_LIBS} image)

