set(SRCS src/embed_static_from_res.cpp src/resource.rc)
add_test_target(embed_static_from_res "${SRCS}")
target_include_directories(embed_static_from_res PRIVATE include)

file(COPY res DESTINATION ${CMAKE_BINARY_DIR}/tests/embed_static_from_res)
