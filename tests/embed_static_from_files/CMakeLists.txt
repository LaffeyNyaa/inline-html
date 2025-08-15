set(SRCS src/embed_static_from_files.cpp)
add_test_target(embed_static_from_files "${SRCS}")
target_include_directories(embed_static_from_files PRIVATE include)

file(COPY res DESTINATION ${CMAKE_BINARY_DIR}/tests/embed_static_from_files)
