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

include_directories(test)
aux_source_directory(test TEST_LIST)

aux_source_directory(test/allocators TEST_LIST)
aux_source_directory(test/containers TEST_LIST)
aux_source_directory(test/table TEST_LIST)
aux_source_directory(test/utils TEST_LIST)

add_executable(test_dsim ${TEST_LIST})
add_dependencies(test_dsim dsim)
target_link_libraries(test_dsim -ldsim)

add_custom_target(tests
    COMMAND find CMakeFiles/dsim.dir/ -name *.gcda -type f -delete
    COMMAND test_dsim
    DEPENDS test_dsim)

add_custom_target(coverage
    COMMAND lcov --capture --directory CMakeFiles/dsim.dir/ --output-file coverage.info
    COMMAND genhtml coverage.info --output-directory coverage
    DEPENDS tests)

############################################################
# Tools
############################################################

OPTION(DSIM_BUILD_TOOLS OFF)

if(DSIM_BUILD_TOOLS)

    aux_source_directory(tools/ddlc DDLC_LIST)
    add_executable(ddlc ${DDLC_LIST})
    add_dependencies(ddlc dsim)
    target_link_libraries(ddlc -ldsim -lflatccrt)

endif(DSIM_BUILD_TOOLS)

############################################################
# Sample
############################################################

OPTION(DSIM_BUILD_SAMPLE OFF)

if(DSIM_BUILD_SAMPLE)

    set(SAMPLE_FILES
        sample/sample.lua
        sample/sample.json)

    add_custom_target(sample_prepare
        #COMMAND ddlc sample.json
        COMMAND cmake -E copy *.lua ${PROJECT_BINARY_DIR}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/sample
        SOURCES ${SAMPLE_FILES})
    add_dependencies(sample_prepare ddlc)

    aux_source_directory(sample SAMPLE_LIST)
    add_executable(sample ${SAMPLE_LIST})
    add_dependencies(sample sample_prepare)
    include_directories(/usr/include/luajit-2.0)
    target_link_libraries(sample -ldsim -lluajit-5.1 -lflatccrt)

endif(DSIM_BUILD_SAMPLE)
