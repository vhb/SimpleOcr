CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ocr_test_json)

SET(JSON_TEST_SOURCES
    ./main.cpp
    ../../src/utils/json/json.cpp
    )

ADD_EXECUTABLE(ocr_test_json ${JSON_TEST_SOURCES})

