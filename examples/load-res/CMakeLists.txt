set(SRCS src/load_res.cpp src/resource.rc)
add_example_target(load_res "${SRCS}")
target_include_directories(load_res PRIVATE include)
