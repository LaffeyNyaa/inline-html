set(SRCS src/load_from_files.cpp)
add_example_target(load_from_files "${SRCS}")

if(MSVC)
  file(COPY res DESTINATION ${CMAKE_BINARY_DIR}/examples/load_from_files/Debug)
  file(COPY res
       DESTINATION ${CMAKE_BINARY_DIR}/examples/load_from_files/Release)
  file(COPY res
       DESTINATION ${CMAKE_BINARY_DIR}/examples/load_from_files/MinSizeRel)
else()
  file(COPY res DESTINATION ${CMAKE_BINARY_DIR}/examples/load_from_files)
endif()
