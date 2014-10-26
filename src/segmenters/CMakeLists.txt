cmake_minimum_required(VERSION 2.6)

project(ocr)
include_directories(..)

set(
    CONTOUR_SEGMENTER_SOURCE
    ./ContoursSegmenter.cpp
    )

set(
    HISTOGRAM_SEGMENTER_SOURCE
    ./HistogramSegmenter.cpp
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g3")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../..)

add_library(ContoursSegmenter SHARED ${CONTOUR_SEGMENTER_SOURCE})
add_library(HistogramSegmenter SHARED ${HISTOGRAM_SEGMENTER_SOURCE})

target_link_libraries(ContoursSegmenter ${OpenCV_LIBS} image)
target_link_libraries(HistogramSegmenter ${OpenCV_LIBS} image)

