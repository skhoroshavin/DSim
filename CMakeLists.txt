project(dsim)
cmake_minimum_required(VERSION 2.8)


############################################################
# dsim
############################################################

add_definitions(-std=c99 -Wall -Wextra)
include_directories(src)

aux_source_directory(src/allocators SRC_LIST)
aux_source_directory(src/containers SRC_LIST)
aux_source_directory(src/table SRC_LIST)
aux_source_directory(src/utils SRC_LIST)

add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS --coverage LINK_FLAGS --coverage)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

############################################################
# Tests
############################################################

enable_testing()

include_directories(test)
aux_source_directory(test TST_LIST)

aux_source_directory(test/allocators TST_LIST)
aux_source_directory(test/containers TST_LIST)
aux_source_directory(test/table TST_LIST)
aux_source_directory(test/utils TST_LIST)

link_directories(${PROJECT_BINARY_DIR})
add_executable(test_${PROJECT_NAME} ${TST_LIST})
add_dependencies(test_${PROJECT_NAME} ${PROJECT_NAME})
target_link_libraries(test_${PROJECT_NAME} -ldsim)

add_test(${PROJECT_NAME} ${PROJECT_BINARY_DIR}/test_${PROJECT_NAME})

############################################################
# Coverage
############################################################

add_custom_target(coverage
    COMMAND test_${PROJECT_NAME}
    COMMAND lcov --capture --directory CMakeFiles/dsim.dir/ --output-file coverage.info
    COMMAND genhtml coverage.info --output-directory coverage
    DEPENDS test_${PROJECT_NAME})
