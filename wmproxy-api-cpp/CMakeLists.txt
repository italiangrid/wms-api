
message("*\n*\n*\n* Configuring build for wmproxy-api-cpp\n*\n*\n*")

set(PROJECT_SOURCE_DIR "${MAIN_PROJECT_SOURCE_DIR}/wmproxy-api-cpp/")

#add_definitions(${CLASSAD_CFLAGS})
  
add_subdirectory(src)
add_subdirectory(interface)
