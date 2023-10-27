cmake_minimum_required(VERSION 3.26)

project (wheel)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-O2)
add_compile_options(-Wall)

add_subdirectory(src)
