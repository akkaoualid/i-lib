cmake_minimum_required(VERSION 3.5)
project(i-lib CXX)

set(compile_hidden OFF)
set(run_examples OFF)
set(run_tests OFF)

if(run_examples)
    add_subdirectory(examples)
elseif(run_tests)
    add_subdirectory(tests)
elseif(compile_hidden)
    add_subdirectory(hidden)
endif()
