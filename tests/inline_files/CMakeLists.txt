set(SRCS src/inline_files.cpp)

add_test_target(inline_files "${SRCS}")

file(COPY res DESTINATION ${CMAKE_BINARY_DIR}/tests/inline_files)
