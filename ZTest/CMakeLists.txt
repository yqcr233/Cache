cmake_minimum_required(VERSION 3.15)
project(TEST)

add_library(test test.cpp)

target_include_directories(test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test PRIVATE lru lru_k hash_lru lfu hash_lfu arc)
