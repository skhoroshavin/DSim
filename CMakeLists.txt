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
	AnyDataStorage.h

	ComponentStorage.h
	ComponentRuntime.h
)

set( SRC_LIST
	DataIndex.cpp

	AnyDataStorage.cpp

	ComponentStorage.cpp
	ComponentRuntime.cpp

	# TOREMOVE
	DataBlock.cpp
)

set( TEST_LIST
	main.cpp

	# TOREMOVE
	test/testDataBlock.cpp
)

add_executable(${PROJECT_NAME} ${HDR_LISR} ${SRC_LIST} ${TEST_LIST})
