cmake_minimum_required(VERSION 2.6)
 
file(GLOB examples "*.c")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

foreach(example_fname ${examples})
	get_filename_component(example ${example_fname} NAME_WE)
 
	add_executable(${example} ${example_fname})
	target_link_libraries(${example} thread)
 
	set(example_pthread ${example}-pthread)
 
	add_executable(${example_pthread} ${example})
	set_target_properties(${example_pthread} PROPERTIES COMPILE_FLAGS "-DUSE_PTHREAD")
	target_link_libraries(${example_pthread} pthread)
 
endforeach()

