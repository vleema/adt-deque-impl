cmake_minimum_required(VERSION 3.5)
project ( Deque VERSION 1.0 LANGUAGES CXX )

  #=== CREATING COMPILING DATABASE ===#
# Currently CMake (since 2.8.5) supports generation of compilation databases
# for Unix Makefile builds (Ninja builds in the works) with the option
# CMAKE_EXPORT_COMPILE_COMMANDS.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#=== SETTING VARIABLES ===#
# Appending to existing flags the correct way (two methods)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Using TestManager Library
# [1] Compile the TestManagere first into a lib.
set(TEST_LIB "TM")
add_library( ${TEST_LIB} STATIC ${CMAKE_CURRENT_SOURCE_DIR}/tm/test_manager.cpp )
target_include_directories( ${TEST_LIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tm )
set_target_properties( ${TEST_LIB} PROPERTIES CXX_STANDARD 17 )

# [2] Setup the executable that will run the tests.
set ( TEST_DRIVER "run_tests")
add_executable( ${TEST_DRIVER} main.cpp iterator_tests.cpp)
set_target_properties( ${TEST_DRIVER} PROPERTIES CXX_STANDARD 17 )
# [3] Link tests compiled sources with the TestManager lib.
target_link_libraries( ${TEST_DRIVER} PRIVATE ${TEST_LIB} )
