cmake_minimum_required(VERSION 2.8.8)

project(machine_learning_ocr)

# Settings
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++0x -g3 -ldl")

find_package("OpenCV")

message("${OpenCV_INCLUDE_PATH}")
message("${OPENCV_FOUND}")
message("${OpenCV_LIBS}")

include_directories(
    ./src
    ${OPENCV2_INCLUDE_DIR}
)

set(
    LIB_SOURCES
)

add_library(OBJECTS OBJECT ${LIB_SOURCES})

# OpenCV application

set(APP_SOURCES
    ./src/main.cpp
    ./src/Brain.cpp
    ./src/utils/json/json.cpp
    ./src/PreprocessorManager.cpp
)

set(IMG_LIB_SOURCES
    ./src/Image.cpp
)

add_executable(ocr ${APP_SOURCES} $<TARGET_OBJECTS:OBJECTS>)
add_library(image SHARED ${IMG_LIB_SOURCES})
target_link_libraries(image ${OpenCV_LIBS})

target_link_libraries(ocr ${OpenCV_LIBS} image)
add_subdirectory(./tests)
add_subdirectory(./src)

