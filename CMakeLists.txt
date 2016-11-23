project(dsim)
cmake_minimum_required(VERSION 2.8)

############################################################
# Coveralls
############################################################

option(COVERALLS "Turn on coveralls support" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

if (COVERALLS)
	include(Coveralls)
	coveralls_turn_on_coverage()
endif()

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
#target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
#target_link_libraries(${PROJECT_NAME} -lgcov)

############################################################
# Tests
############################################################

if (COVERALLS)
    coveralls_setup("${SRC_LIST}" ${COVERALLS_UPLOAD})
endif()

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
