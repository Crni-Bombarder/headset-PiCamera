cmake_minimum_required(VERSION 2.8)
project(headset-PiCamera)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenCV REQUIRED)
include_directories(raspberry/includes)
include_directories(odroid/includes)
file(
        GLOB_RECURSE
        source_files
        raspberry/*
        odroid/*
)

add_library(RASPBERRY raspberry/src/VideoGrab.cpp)
target_link_libraries(RASPBERRY ${OpenCV_LIBS})

add_executable(TestVideoGrab raspberry/TestVideoGrab.cpp)
target_link_libraries(TestVideoGrab ${OpenCV_LIBS} RASPBERRY)
