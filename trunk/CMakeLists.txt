cmake_minimum_required(VERSION 2.6)
project(projetOS)

set(CUSTOM_FLAGS "" CACHE FILEPATH "User defined compilation flags")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CUSTOM_FLAGS}")

# Write binaries to bin/
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

set(PROJECT_INCLUDE_DIR "include")
include_directories("${PROJECT_INCLUDE_DIR}")

set(USE_PTHREAD FALSE CACHE BOOL "pthread is used if checked")
if(USE_PTHREAD)
  message("Using pthread")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_PTHREAD")
  set(THREAD_LIB "pthread")
else()
  message("Using user threads")

  file(GLOB_RECURSE thread_files
       "src/*.c"
       "src/*.h"
       "include/thread.h")

  add_library("thread"
	${thread_files})

  set(THREAD_LIB "thread")
endif()

add_executable("main"
	test/main.c)

add_executable("test"
	test/test.c)
target_link_libraries("test"
        ${THREAD_LIB})

add_executable("01-main"
	test/01-main.c)
target_link_libraries("01-main"
        ${THREAD_LIB})

add_executable("02-switch"
	test/02-switch.c)
target_link_libraries("02-switch"
        ${THREAD_LIB})

add_executable("51-fibonacci"
	test/51-fibonacci.c)
target_link_libraries("51-fibonacci"
        ${THREAD_LIB})

add_executable("11-join"
  test/11-join.c)
target_link_libraries("11-join"
        ${THREAD_LIB})

add_executable("12-join-main"
  test/12-join-main.c)
target_link_libraries("12-join-main"
        ${THREAD_LIB})

add_executable("21-create-many"
  test/21-create-many.c)
target_link_libraries("21-create-many"
        ${THREAD_LIB})

add_executable("22-create-many-recursive"
  test/22-create-many-recursive.c)
target_link_libraries("22-create-many-recursive"
        ${THREAD_LIB})

add_executable("31-switch-many"
  test/31-switch-many.c)
target_link_libraries("31-switch-many"
        ${THREAD_LIB})

add_executable("32-switch-many-join"
  test/32-switch-many-join.c)
target_link_libraries("32-switch-many-join"
        ${THREAD_LIB})

#######
##Tests
#######
include(CTest)
enable_testing()

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