cmake_minimum_required(VERSION 2.6)

project(ocr)
include_directories(..)

set(
    TEST_CLASSIFIER_SOURCE
    ./TestClassifier.cpp
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g3")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../..)

add_library(TestClassifier SHARED ${TEST_CLASSIFIER_SOURCE})
target_link_libraries(TestClassifier ${OpenCV_LIBS} image)

