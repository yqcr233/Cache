cmake_minimum_required(VERSION 3.15)
project(HASHLFU)

add_library(hash_lfu hash_lfu.cpp)

target_include_directories(hash_lfu PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(hash_lfu PRIVATE lfu)