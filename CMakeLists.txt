cmake_minimum_required(VERSION 2.8.8)

project(machine_learning_ocr)

# Settings
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
if (APPLE)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++1y -g3")
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++1y -g3 -ldl")
endif()

find_package("OpenCV")

include_directories(
    ./src
    ${OPENCV2_INCLUDE_DIR}
)

# OpenCV application

set(APP_SOURCES
    ./src/main.cpp
    ./src/Brain.cpp
    ./src/PreprocessorManager.cpp
)

set(IMG_LIB_SOURCES
    ./src/Image.cpp
    ./src/Dataset.cpp
    ./src/utils/json/json.cpp
)

add_executable(ocr ${APP_SOURCES} )
add_library(image SHARED ${IMG_LIB_SOURCES})
target_link_libraries(image ${OpenCV_LIBS})

target_link_libraries(ocr ${OpenCV_LIBS} image)
add_subdirectory(./tests)
add_subdirectory(./src)

