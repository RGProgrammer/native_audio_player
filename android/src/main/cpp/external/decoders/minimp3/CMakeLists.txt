cmake_minimum_required(VERSION 3.10.0)

project(minimp3)
add_definitions(-DMINIMP3_IMPLEMENTATION -DMINIMP3_FLOAT_OUTPUT)
#setSources
include_directories(".")
set(minisources
        minimp3.h
        minimp3_ex.h
        minimp3.cpp
        minimp3_ex.cpp)


add_library(minimp3 STATIC ${minisources})