set(SRCS src/inline_html.cpp)

add_library(inline_html ${SRCS})
set_target_properties(inline_html PROPERTIES CXX_STANDARD 20)
target_include_directories(inline_html PRIVATE include)
target_compile_features(inline_html PRIVATE cxx_std_20)

if(MSVC)
  target_compile_options(inline_html PRIVATE /W4)
  target_compile_definitions(inline_html PRIVATE _HAS_EXCEPTIONS=0)
endif()

if(WIN32)
  target_compile_definitions(inline_html PUBLIC WIN32)
endif()
