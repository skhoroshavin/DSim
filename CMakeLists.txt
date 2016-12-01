project(dsim)
cmake_minimum_required(VERSION 2.8)

############################################################
# General config
############################################################

add_definitions(-Wall -Wextra)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c99")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")

############################################################
# dsim
############################################################

include_directories(src)

aux_source_directory(src/allocators SRC_LIST)
aux_source_directory(src/containers SRC_LIST)
aux_source_directory(src/table SRC_LIST)
aux_source_directory(src/utils SRC_LIST)

add_library(dsim SHARED ${SRC_LIST})
link_directories(${PROJECT_BINARY_DIR})

############################################################
# Tests
############################################################

enable_testing()

include_directories(test)
aux_source_directory(test TEST_LIST)

aux_source_directory(test/allocators TEST_LIST)
aux_source_directory(test/containers TEST_LIST)
aux_source_directory(test/table TEST_LIST)
aux_source_directory(test/utils TEST_LIST)

add_executable(test_dsim ${TEST_LIST})
add_dependencies(test_dsim dsim)
target_link_libraries(test_dsim -ldsim)

add_test(dsim ${PROJECT_BINARY_DIR}/test_dsim)

############################################################
# Coverage
############################################################

add_custom_target(coverage
    COMMAND find CMakeFiles/dsim.dir/ -name *.gcda -type f -delete
    COMMAND test_dsim
    COMMAND lcov --capture --directory CMakeFiles/dsim.dir/ --output-file coverage.info
    COMMAND genhtml coverage.info --output-directory coverage
    DEPENDS test_dsim)

############################################################
# Sample
############################################################

if(DSIM_BUILD_SAMPLE)

    aux_source_directory(sample SAMPLE_LIST)
    add_executable(sample ${SAMPLE_LIST})
    add_custom_command(TARGET sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/sample/*.lua ${PROJECT_BINARY_DIR}
        DEPENDS dsim)
    target_link_libraries(sample -ldsim -llua)

endif(DSIM_BUILD_SAMPLE)
