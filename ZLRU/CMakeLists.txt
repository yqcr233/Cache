cmake_minimum_required(VERSION 3.15)
project(LRU)

add_library(lru lru.cpp)

target_include_directories(lru PRIVATE ${CMAKE_SOURCE_DIR}/include)
