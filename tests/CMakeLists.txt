add_subdirectory(load_html_from_file)

if(WIN32)
  add_subdirectory(load_html_from_res)
endif()

add_subdirectory(embed_static_from_files)