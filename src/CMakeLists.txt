cmake_minimum_required(VERSION 2.6)

project(ocr)

set(EXECUTABLE_OUTPUT_PATH .)

add_subdirectory(./preprocessors)
add_subdirectory(./feature_extractors)
add_subdirectory(./classifiers)
add_subdirectory(./segmenters)
