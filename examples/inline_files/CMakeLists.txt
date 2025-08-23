file(GLOB_RECURSE SRCS "src/*")
add_example_target("inline_files" "${SRCS}")

file(COPY "res" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
