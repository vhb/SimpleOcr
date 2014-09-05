cmake_minimum_required(VERSION 2.8.8)

project(machine_learning_ocr)

# Macros
#macro(unit_test NAME)
  #message(STATUS "Add unit test ${NAME}")
  #add_executable(test${NAME} tests/${NAME}.cpp $<TARGET_OBJECTS:OBJECTS>)
  #target_link_libraries(test${NAME} ${OpenCV_LIBS})
#endmacro()

# Settings

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++0x")
find_package(OpenCV REQUIRED)

include_directories(
    ./libs
    ${OPENCV_INCLUDE_DIR}
)

set(
    LIB_SOURCES
)

add_library(OBJECTS OBJECT ${LIB_SOURCES})

# OpenCV application

set(APP_SOURCES
    ./app/main.cpp
)

add_executable(ocr ${APP_SOURCES} $<TARGET_OBJECTS:OBJECTS>)

target_link_libraries(ocr ${OpenCV_LIBS})

# Some unit tests
#unit_test(PatternFinder)


