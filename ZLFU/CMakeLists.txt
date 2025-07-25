cmake_minimum_required(VERSION 3.15)
project(LFU)

add_library(lfu lfu.cpp)

target_include_directories(lfu PRIVATE ${CMAKE_SOURCE_DIR}/include)