cmake_minimum_required(VERSION 3.15)
project(ARC)

add_library(arc arc.cpp arc_lfu.cpp arc_lru.cpp)

target_include_directories(arc PRIVATE ${CMAKE_SOURCE_DIR}/include)