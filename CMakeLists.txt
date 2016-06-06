project(DSim)
cmake_minimum_required(VERSION 2.8)
add_definitions(-std=c++14)
include_directories(.)

aux_source_directory(. SRC_LIST)
aux_source_directory(./test TEST_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${TEST_LIST})
