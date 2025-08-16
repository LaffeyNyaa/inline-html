set(SRCS src/html_static_embedder.cpp)
set(LIB html_static_embedder)

add_library(${LIB} ${SRCS})
set_target_properties(${LIB} PROPERTIES CXX_STANDARD 20)
target_include_directories(${LIB} PRIVATE include)
target_compile_features(${LIB} PRIVATE cxx_std_20)

if(MSVC)
  target_compile_options(${LIB} PRIVATE /W4)
  target_compile_definitions(${LIB} PRIVATE _HAS_EXCEPTIONS=0)
endif()

if(WIN32)
  target_compile_definitions(${LIB} PUBLIC WIN32)
endif()
