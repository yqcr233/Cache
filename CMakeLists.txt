cmake_minimum_required(VERSION 3.15)
project(CACHE_)

set(SRC_LIST main.cpp)

add_subdirectory(ZLRU)
add_subdirectory(ZLRUK)
add_subdirectory(ZARC)
add_subdirectory(ZLFU)
add_subdirectory(ZHASHLFU)
add_subdirectory(ZHASHLRU)
add_subdirectory(ZTest)

add_executable(t_cache ${SRC_LIST})

target_link_libraries(t_cache PRIVATE 
    lru 
    lru_k
    lfu
    arc
    hash_lfu
    hash_lru
    test
)
target_include_directories(t_cache PRIVATE ${CMAKE_SOURCE_DIR}/include)