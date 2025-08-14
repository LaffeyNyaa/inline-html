macro(add_example_target NAME SRCS)
  add_executable(${NAME} ${SRCS})
  set_target_properties(${NAME} PROPERTIES CXX_STANDARD 20)
  target_include_directories(${NAME} PRIVATE ${HTML_STATIC_EMBEDDER_INCLUDE_DIR}
                                             include)
  target_link_libraries(${NAME} PRIVATE html_static_embedder)

  if(MSVC)
    target_compile_options(${NAME} PRIVATE /W4)
  endif()
endmacro()

macro(add_test_target NAME SRCS)
  add_example_target(${NAME} "${SRCS}")
  add_test(${NAME} ${NAME})
endmacro()
