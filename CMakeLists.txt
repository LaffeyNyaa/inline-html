cmake_minimum_required(VERSION 3.31)
project(inline-html)

enable_testing()

include(cmake/vars.cmake)
include(cmake/macros.cmake)

add_subdirectory(core)
add_subdirectory(examples)
add_subdirectory(tests)
