set(SRCS src/load_from_files.cpp)
add_example_target(load_from_files "${SRCS}")

file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
