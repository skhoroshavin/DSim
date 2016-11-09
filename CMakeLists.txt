project(DSim)
cmake_minimum_required(VERSION 2.8)

#add_subdirectory(3rdparty/criterion)
#include_directories(3rdparty/criterion/include)
#link_directories(${PROJECT_BINARY_DIR}/3rdparty/criterion)

include_directories(src)
add_definitions(-std=c99)

aux_source_directory(src/allocators SRC_LIST)
aux_source_directory(src/containers SRC_LIST)
aux_source_directory(src/utils SRC_LIST)
aux_source_directory(test/containers SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} -lcriterion)
#add_dependencies(${PROJECT_NAME} criterion)
