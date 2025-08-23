set(SRCS "src/inline_res.cpp" "src/resource.rc")

add_test_target("inline_res" "${SRCS}")
target_include_directories("inline_res" PRIVATE "include")
