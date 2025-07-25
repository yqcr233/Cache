cmake_minimum_required(VERSION 3.15)
project(LRU_K)

add_library(lru_k lru_k.cpp)

target_include_directories(lru_k PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(lru_k PRIVATE lru)