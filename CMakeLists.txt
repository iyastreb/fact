cmake_minimum_required (VERSION 2.8)
project (fact)

SET(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory (src)

add_custom_target(test
    COMMAND BIN=src/fact python test/fact_test.py)
