set(SRCS "src/load_from_res.cpp" "src/resource.rc")
add_example_target("load_from_res" "${SRCS}")
target_include_directories("load_from_res" PRIVATE "include")
