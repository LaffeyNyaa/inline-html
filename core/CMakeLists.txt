set(SRCS src/html_static_embed.cpp)
set(TARGET html_static_embed)

add_library(${TARGET} ${SRCS})
set_target_properties(${TARGET} PROPERTIES CXX_STANDARD 20)
target_include_directories(${TARGET} PRIVATE include)
target_compile_features(${TARGET} PRIVATE cxx_std_20)

if(MSVC)
  target_compile_options(${TARGET} PRIVATE /W4)
  target_compile_definitions(${TARGET} PRIVATE _HAS_EXCEPTIONS=0)
endif()

if(WIN32)
  target_compile_definitions(${TARGET} PUBLIC WIN32)
endif()
