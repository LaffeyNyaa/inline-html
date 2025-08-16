set(SRCS src/inline_html.cpp)

add_library(${INLINE_HTML} ${SRCS})
set_target_properties(${INLINE_HTML} PROPERTIES CXX_STANDARD 20)
target_include_directories(${INLINE_HTML} PRIVATE include)
target_compile_features(${INLINE_HTML} PRIVATE cxx_std_20)

if(MSVC)
  target_compile_options(${INLINE_HTML} PRIVATE /W4)
  target_compile_definitions(${INLINE_HTML} PRIVATE _HAS_EXCEPTIONS=0)
endif()

if(WIN32)
  target_compile_definitions(${INLINE_HTML} PUBLIC WIN32)
endif()
