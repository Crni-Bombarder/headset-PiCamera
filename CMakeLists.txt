cmake_minimum_required(VERSION 2.8)
project(headset-PiCamera)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(FFmpeg REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)

include_directories(common/includes
                    raspberry/includes
                    odroid/includes
                    ${LIBAVUTIL_INCLUDE_DIR}
                    ${LIBAVCODEC_INCLUDE_DIR}
                    ${LIBAVFORMAT_INCLUDE_DIR}
                    ${LIBAVDEVICE_INCLUDE_DIR}
                    ${SDL2_INCLUDE_DIRS})

file(
        GLOB_RECURSE
        source_files
        common/*
)

add_library(COMMON  common/Frame.c
                    common/Packet.c)
#target_link_libraries(COMMON)

add_library(RASPBE  raspberry/Raspberry.c
                    raspberry/ServerTCP.c)
target_link_libraries(RASPBE COMMON -lavformat -lavcodec -lavutil -lavdevice -lswscale)

add_library(ODROID  odroid/Odroid.c
                    odroid/ClientTCP.c
                    odroid/Display.c)
target_link_libraries(ODROID COMMON -lSDL2 -lavformat -lavcodec -lavutil -lavdevice -lswscale -lswresample ${CMAKE_THREAD_LIBS_INIT})

add_executable(TestCapture TestCapture.c)
target_link_libraries(TestCapture COMMON)

add_executable(RaspberryMain RaspberryMain.c)
target_link_libraries(RaspberryMain RASPBE)

add_executable(OdroidMain OdroidMain.c)
target_link_libraries(OdroidMain ODROID)

add_executable(TestAudioOdroid TestAudioOdroid.c)
target_link_libraries(TestAudioOdroid ODROID)
