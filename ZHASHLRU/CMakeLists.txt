cmake_minimum_required(VERSION 3.15)
project(HASHLRU)

add_library(hash_lru hash_lru.cpp)

target_include_directories(hash_lru PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(hash_lru PRIVATE lru lru_k)