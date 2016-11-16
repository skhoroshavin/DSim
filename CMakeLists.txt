project(dsim)
cmake_minimum_required(VERSION 2.8)

############################################################
# dsim
############################################################

include_directories(src)
add_definitions(-std=c99)

aux_source_directory(src/allocators SRC_LIST)
aux_source_directory(src/containers SRC_LIST)
aux_source_directory(src/table SRC_LIST)
aux_source_directory(src/utils SRC_LIST)

add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

############################################################
# Tests
############################################################

aux_source_directory(test/allocators TST_LIST)
aux_source_directory(test/containers TST_LIST)
aux_source_directory(test/table TST_LIST)
aux_source_directory(test/utils TST_LIST)

link_directories(${PROJECT_BINARY_DIR})
add_executable(test_${PROJECT_NAME} ${TST_LIST})
add_dependencies(test_${PROJECT_NAME} ${PROJECT_NAME})
target_link_libraries(test_${PROJECT_NAME} -ldsim -lcriterion)
