cmake_minimum_required(VERSION 3.26)

project (wheel)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-O2)
add_compile_options(-Wall -Werror)
# add_compile_options(-Wglobal-constructors)

add_subdirectory(src)
