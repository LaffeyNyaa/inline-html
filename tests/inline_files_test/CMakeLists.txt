set(SRCS "src/inline_files_test.cpp")

add_test_target("inline_files_test" "${SRCS}")

file(COPY "res" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
