cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
# set project name and version 
project(MemoryPool VERSION 1.0)

add_executable(MemoryPool
    src/main.cpp
    src/structures/memPool.cpp
    src/structures/lockFreeQueue.cpp
)

target_compile_options(MemoryPool PRIVATE
    -fsanitize=address,undefined
    -fno-omit-frame-pointer
    -Wall 
    -Wextra 
    -Wshadow 
    -Wconversion 
    -Wpedantic 
    -Werror
)

target_link_options(MemoryPool PRIVATE
    -fsanitize=address,undefined
)