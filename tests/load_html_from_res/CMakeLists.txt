set(SRCS src/load_html_from_res.cpp src/resource.rc)
add_test_target(load_html_from_res "${SRCS}")
target_include_directories(load_html_from_res PRIVATE include)