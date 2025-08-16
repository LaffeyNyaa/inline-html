cmake_minimum_required(VERSION 4.0)
project(html-static-embed)

enable_testing()

include(cmake/vars.cmake)
include(cmake/macros.cmake)

add_subdirectory(core)
add_subdirectory(examples)
add_subdirectory(tests)
