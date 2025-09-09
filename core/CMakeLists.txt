file(GLOB_RECURSE SRCS "src/*")

add_library("${PROJECT_NAME}" "${SRCS}")
target_include_directories("${PROJECT_NAME}" PUBLIC "include")
target_compile_features("${PROJECT_NAME}" PRIVATE "cxx_std_20")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options("${PROJECT_NAME}" PRIVATE "-Wall")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options("${PROJECT_NAME}" PRIVATE "/W4")
endif()
