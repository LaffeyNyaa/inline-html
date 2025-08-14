set(SRCS src/load_html_from_file.cpp src/resource.rc)
add_test_target(load_html_from_file "${SRCS}")
target_include_directories(load_html_from_file PRIVATE include)

file(COPY res DESTINATION ${CMAKE_BINARY_DIR}/tests/load_html_from_file)
