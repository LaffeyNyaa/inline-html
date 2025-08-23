set(SRCS "src/inline_res_test.cpp" "src/resource.rc")

add_test_target("inline_res_test" "${SRCS}")
target_include_directories("inline_res_test" PRIVATE "include")
