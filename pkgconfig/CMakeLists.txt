configure_file(${CMAKE_CURRENT_SOURCE_DIR}/wms-api.pc.in
               ${CMAKE_CURRENT_BINARY_DIR}/wms-api.pc
               @ONLY)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
  DESTINATION ${PREFIX}/usr/${LIBDIR}/pkgconfig
  FILES_MATCHING PATTERN "wms-api.pc"
  PATTERN "CMakeFiles" EXCLUDE)
