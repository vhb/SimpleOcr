cmake_minimum_required(VERSION 2.6)

project(ocr)
include_directories(..)


set(
    TEST_EXTRACTOR_SOURCE
    ./TestExtractor.cpp
    )
set(
    MOMENT_EXTRACTOR_SOURCE
    ./MomentExtractor.cpp
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g3")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../..)

add_library(TestExtractor SHARED ${TEST_EXTRACTOR_SOURCE})
target_link_libraries(TestExtractor ${OpenCV_LIBS} image)

add_library(MomentExtractor SHARED ${MOMENT_EXTRACTOR_SOURCE})
target_link_libraries(MomentExtractor ${OpenCV_LIBS} image)


