cmake_minimum_required(VERSION 2.6)
project(projetOS)

set(CUSTOM_FLAGS "" CACHE FILEPATH "User defined compilation flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CUSTOM_FLAGS}")

# Write binaries to bin/
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

file(GLOB_RECURSE SRC_FILES
       "src/*.c"
       "include/*.h")

set(PROJECT_INCLUDE_DIR "include")
include_directories("${PROJECT_INCLUDE_DIR}")

add_executable("test1"
	test/main.c)

add_executable("test2"
        test/test.c
	${SRC_FILES})

