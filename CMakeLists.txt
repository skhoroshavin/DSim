project(DSim)
cmake_minimum_required(VERSION 2.8)
add_definitions(-std=c++14)
include_directories(.)

set( HDR_LIST
	json.h

	DSimCommon.h

	DataIndex.h
	DataStorage.h

	DataType.h
	GenericDataStorage.h

	ComponentStorage.h
	ComponentRuntime.h
)

set( SRC_LIST
	DataIndex.cpp

	GenericDataStorage.cpp

	ComponentStorage.cpp
	ComponentRuntime.cpp
)

set( TEST_LIST
	test/main.cpp

	test/testGenericDataStorage.cpp
	test/testComponentStorage.cpp
)

add_executable(${PROJECT_NAME} ${HDR_LISR} ${SRC_LIST} ${TEST_LIST})
