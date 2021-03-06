project(dsim)
cmake_minimum_required(VERSION 2.8)

############################################################
# General config
############################################################

add_definitions(-Wall -Wextra -Werror)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c99")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")
set(FLATCC_RTONLY On)
set(FLATCC_REFLECTION Off)

add_subdirectory(3rdparty/flatcc)
include_directories(3rdparty/flatcc/include)
link_directories(${PROJECT_SOURCE_DIR}/3rdparty/flatcc/lib)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_custom_target(dsim_flatcc
        COMMAND cmake -E copy lib/libflatccrt_d.a lib/libflatccrt.a
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/3rdparty/flatcc
        DEPENDS flatccrt )
else(CMAKE_BUILD_TYPE MATCHES Debug)
    add_custom_target(dsim_flatcc
        DEPENDS flatccrt )
endif(CMAKE_BUILD_TYPE MATCHES Debug)

############################################################
# dsim
############################################################

include_directories(src)

aux_source_directory(src/core SRC_LIST)
aux_source_directory(src/test SRC_LIST)
aux_source_directory(src/storage SRC_LIST)

add_library(dsim SHARED ${SRC_LIST})
add_dependencies(dsim dsim_flatcc)
link_directories(${PROJECT_BINARY_DIR})

############################################################
# Tests
############################################################

add_custom_target(test_dsim_prepare
    COMMAND ddlc test.ddl
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test/storage
    SOURCES test/storage/test.ddl)
add_dependencies(test_dsim_prepare ddlc)

include_directories(test)
aux_source_directory(test TEST_LIST)

aux_source_directory(test/core TEST_LIST)
aux_source_directory(test/test TEST_LIST)
aux_source_directory(test/storage TEST_LIST)

add_executable(test_dsim ${TEST_LIST})
add_dependencies(test_dsim test_dsim_prepare)
target_link_libraries(test_dsim -ldsim)

add_custom_target(tests
    COMMAND valgrind ./test_dsim
    DEPENDS test_dsim)

add_custom_target(coverage
    COMMAND lcov --capture --directory CMakeFiles/dsim.dir/ --output-file coverage.info
    COMMAND genhtml coverage.info --output-directory coverage
    DEPENDS tests)

add_custom_target(coverage_clean
    COMMAND find CMakeFiles/dsim.dir/ -name *.gcda -type f -delete || true
    COMMAND rm -rvf coverage*)

############################################################
# Tools
############################################################

aux_source_directory(tools/ddlc DDLC_LIST)
add_executable(ddlc ${DDLC_LIST})
add_dependencies(ddlc dsim)
target_link_libraries(ddlc -ldsim -lflatccrt)

############################################################
# Sample
############################################################

OPTION(DSIM_BUILD_SAMPLE OFF)

if(DSIM_BUILD_SAMPLE)

    set(SAMPLE_FILES
        sample/sample.lua
        sample/sample.ddl)

    add_custom_target(sample_prepare
        COMMAND ddlc sample.ddl
        COMMAND cmake -E copy *.lua ${PROJECT_BINARY_DIR}
        COMMAND cmake -E copy *.bin ${PROJECT_BINARY_DIR}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/sample
        SOURCES ${SAMPLE_FILES})
    add_dependencies(sample_prepare ddlc)

    aux_source_directory(sample SAMPLE_LIST)
    add_executable(sample ${SAMPLE_LIST})
    add_dependencies(sample sample_prepare)
    include_directories(/usr/include/luajit-2.0)
    target_link_libraries(sample -ldsim -lluajit-5.1 -lflatccrt)

endif(DSIM_BUILD_SAMPLE)
