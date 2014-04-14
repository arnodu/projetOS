cmake_minimum_required(VERSION 2.6)
project(projetOS)

set(CUSTOM_FLAGS "" CACHE FILEPATH "User defined compilation flags")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} ${CUSTOM_FLAGS}")

# Write binaries to bin/
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

file(GLOB_RECURSE thread_files
       "src/*.c"
       "src/*.h"
       "include/thread.h")

include(CTest)
enable_testing()


set(PROJECT_INCLUDE_DIR "include")
include_directories("${PROJECT_INCLUDE_DIR}")

add_library(thread
	${thread_files})

add_executable("main"
	test/main.c)

add_executable("test"
	test/test.c)
target_link_libraries("test"
        thread)

add_executable("01-main"
	test/01-main.c)
target_link_libraries("01-main"
        thread)

add_executable("02-switch"
	test/02-switch.c)
target_link_libraries("02-switch"
        thread)

add_executable("51-fibonacci"
	test/51-fibonacci.c)
target_link_libraries("51-fibonacci"
        thread)

add_executable("11-join"
  test/11-join.c)
target_link_libraries("11-join"
        thread)

add_executable("12-join-main"
  test/12-join-main.c)
target_link_libraries("12-join-main"
        thread)

add_executable("21-create-many"
  test/21-create-many.c)
target_link_libraries("21-create-many"
        thread)

add_executable("22-create-many-recursive"
  test/22-create-many-recursive.c)
target_link_libraries("22-create-many-recursive"
        thread)

add_executable("31-switch-many"
  test/31-switch-many.c)
target_link_libraries("31-switch-many"
        thread)

add_executable("32-switch-many-join"
  test/32-switch-many-join.c)
target_link_libraries("32-switch-many-join"
        thread)

## valgrind
find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)
#INCLUDE(Dart)

####### Adding test rules. ###########
add_test("main_test"
  ${EXECUTABLE_OUTPUT_PATH}/01-main)

add_test("switch_test"
  ${EXECUTABLE_OUTPUT_PATH}/02-switch)

add_test("fibo_10_test"
  ${EXECUTABLE_OUTPUT_PATH}/51-fibonacci 10)

add_test("join_test"
  ${EXECUTABLE_OUTPUT_PATH}/11-join)

add_test("join_main_test"
  ${EXECUTABLE_OUTPUT_PATH}/12-join-main)

add_test("create-many-test"
  ${EXECUTABLE_OUTPUT_PATH}/21-create-many 1001)

add_test("create-many-recursive-test"
  ${EXECUTABLE_OUTPUT_PATH}/22-create-many-recursive 375) # 400 ça marche plus

add_test("switch-many-test"
  ${EXECUTABLE_OUTPUT_PATH}/31-switch-many 10 10)

add_test("switch-many-join-test"
  ${EXECUTABLE_OUTPUT_PATH}/32-switch-many-join 10 10)

add_test("fibo_10_test"
  ${EXECUTABLE_OUTPUT_PATH}/51-fibonacci 10)



