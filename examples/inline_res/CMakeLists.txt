file(GLOB_RECURSE SRCS "src/*")
add_example_target("inline_res" "${SRCS}")
target_include_directories("inline_res" PRIVATE "include")
