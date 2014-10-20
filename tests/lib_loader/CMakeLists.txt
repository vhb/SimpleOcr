cmake_minimum_required(VERSION 2.6)

project(ocr_test_lib)
include_directories(.)

set(
    KOALA_SOURCE
    ./koala.cpp
    )
set(
    PANDA_SOURCE
    ./panda.cpp
    )
set(EXE_TESTS_SOURCES ./lib.cpp)

add_executable(test_lib ${EXE_TESTS_SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g3")
add_library(koala SHARED ${KOALA_SOURCE})
add_library(panda SHARED ${PANDA_SOURCE})
set(EXECUTABLE_OUTPUT_PATH .)

