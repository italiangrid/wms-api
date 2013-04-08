add_custom_command(
  OUTPUT 
  ${PROJECT_SOURCE_DIR}/src/autogen/soapC.cpp 
  ${PROJECT_SOURCE_DIR}/src/autogen/soapClient.cpp
  COMMAND mkdir -p ${PROJECT_SOURCE_DIR}/src/autogen
  COMMAND
  wsdl2h -o ${PROJECT_SOURCE_DIR}/src/autogen/wm.h 
         -t ${PROJECT_SOURCE_DIR}/src/wmptypemap.dat 
         ${PROJECT_SOURCE_DIR}/src/WMProxy.wsdl
  COMMAND 
  soapcpp2 -d${PROJECT_SOURCE_DIR}/src/autogen
-I/usr/include:/usr/import:/usr/share/gsoap/import:/usr/include/gsoap
${PROJECT_SOURCE_DIR}/src/autogen/wm.h
  )


add_custom_target(GENSTUBS ALL DEPENDS 
  ${PROJECT_SOURCE_DIR}/src/autogen/soapC.cpp 
  ${PROJECT_SOURCE_DIR}/src/autogen/soapClient.cpp
  )



include_directories(${CLASSAD_INCLUDE_DIRS}
                    ${PROJECT_SOURCE_DIR}/src/autogen
                    ${PROJECT_SOURCE_DIR}/interface
		    ${PROJECT_SOURCE_DIR}/src)



#add_custom_target(WMPROXYAPI DEPENDS 
#  wm.h 
#  soapC.cpp 
#  soapClient.cpp 
#  wmproxy_api.cpp 
#  wmproxy_api_utilities.cpp 
#  dom_${GSOAP_VERSION}.cpp )

add_library(glite_wms_wmproxy_api_cpp SHARED 
  autogen/soapC.cpp
  autogen/soapClient.cpp 
  wmproxy_api.cpp 
  wmproxy_api_utilities.cpp 
  dom_${GSOAP_VERSION}.cpp )

target_link_libraries(glite_wms_wmproxy_api_cpp
  ${GRIDSITE_OPENSSL_LIBRARIES}
  )

set_target_properties(glite_wms_wmproxy_api_cpp PROPERTIES VERSION 0.0.0 SOVERSION 0)

install(TARGETS glite_wms_wmproxy_api_cpp
  LIBRARY DESTINATION ${PREFIX}/usr/${LIBDIR} )
