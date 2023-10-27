
include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. SRC_LIST)
aux_source_directory(./utility SRC_LIST)
aux_source_directory(./basic SRC_LIST)

message(STATUS ${SRC_LIST})
message(STATUS "source dir: ${PROJECT_SOURCE_DIR}")

add_compile_definitions(WHEEL_LOG_LEVEL=3)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(wheel STATIC ${SRC_LIST})
