set(SRCS src/html_static_embedder.cpp)

add_library(html_static_embedder ${SRCS})
set_target_properties(html_static_embedder PROPERTIES CXX_STANDARD 20)
target_include_directories(html_static_embedder PRIVATE include)

if(MSVC)
  target_compile_options(html_static_embedder PRIVATE /W4)

endif()

if(WIN32)
  target_compile_definitions(html_static_embedder PUBLIC WIN32)
endif()
