cmake_minimum_required(VERSION 3.26)

project (wheel)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -O2)
add_compile_options(-Wall)
add_compile_options(-DDEBUG_TRACE_POINT=1)

add_subdirectory(src)
