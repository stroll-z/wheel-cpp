cmake_minimum_required(VERSION 3.26)

project(SAMPLE)

set(SRC_LIST $ENV{SAMPLE_SRC})

set(CMAKE_CXX_STANDARD 14)
#add_compile_options(-Wall)
add_compile_definitions(WHEEL_LOG_LEVEL=4)

message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "SAMPLE_SRC: ${SAMPLE_SRC}")

include_directories(${PROJECT_SOURCE_DIR}/../include)

link_directories(${PROJECT_SOURCE_DIR}/../lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
add_executable(demo ${SRC_LIST})
target_link_libraries(demo wheel m)
