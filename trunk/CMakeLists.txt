cmake_minimum_required(VERSION 2.6)
project(projetOS)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

set(PROJECT_INCLUDE_DIR "include")
include_directories("${PROJECT_INCLUDE_DIR}")

file(GLOB_RECURSE thread_files
       "src/*.c"
       "src/*.h"
       "include/thread.h")

add_library("thread"
	${thread_files})


add_subdirectory(test)


### test ###


include(CTest)
enable_testing()

## valgrind
find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)

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
  
#add_test("81-preemption"
#  ${EXECUTABLE_OUTPUT_PATH}/preemption)
