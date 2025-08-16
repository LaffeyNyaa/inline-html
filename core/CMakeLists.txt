set(SRCS src/html_static_embedder.cpp)
set(HSE html_static_embedder)

add_library(${HSE} ${SRCS})
set_target_properties(${HSE} PROPERTIES CXX_STANDARD 20)
target_include_directories(${HSE} PRIVATE include)
target_compile_features(${HSE} PRIVATE cxx_std_20)

if(MSVC)
  target_compile_options(${HSE} PRIVATE /W4)
  target_compile_definitions(${HSE} PRIVATE _HAS_EXCEPTIONS=0)
endif()

if(WIN32)
  target_compile_definitions(${HSE} PUBLIC WIN32)
endif()
