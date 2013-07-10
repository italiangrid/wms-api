project(wms-api C CXX)
cmake_minimum_required(VERSION 2.6)

if(NOT ${CMAKE_C_COMPILER} STREQUAL "CMAKE_C_COMPILER-NOTFOUND")
  set(OK_C_COMPILER "1")
endif(NOT ${CMAKE_C_COMPILER} STREQUAL "CMAKE_C_COMPILER-NOTFOUND")

if(NOT ${CMAKE_CXX_COMPILER} STREQUAL "CMAKE_CXX_COMPILER-NOTFOUND")
  set(OK_CXX_COMPILER "1")
endif(NOT ${CMAKE_CXX_COMPILER} STREQUAL "CMAKE_CXX_COMPILER-NOTFOUND")

EXEC_PROGRAM(/bin/date ARGS +'%a %b %d %Y' OUTPUT_VARIABLE DATE)

if(NOT DEFINED OS)
    message(FATAL_ERROR " Undefined Operating System name. Please use -DOS=<os_name>
to customize (choices are \"sl5\", \"sl6\" )")
endif(NOT DEFINED OS)

if(NOT DEFINED MOCKCFG)
    message(WARNING " Undefined mock configuration name. Please use
-DMOCKCFG=<name> for building in mock. Setting to 'none'")
endif(NOT DEFINED MOCKCFG)

if(NOT DEFINED AGE)
    message(WARNING " Undefined age number, selected 0 Please use -DAGE=<number> to customize")
    set(AGE "0")
endif(NOT DEFINED AGE)

if(NOT DEFINED PVER)
    message(WARNING " Undefined version number, selected 0.0.0. Please use
-DPVER=<version> to customize")
    set(PVER "0.0.0")
endif(NOT DEFINED PVER)

if(NOT DEFINED PREFIX)
    message(WARNING " Undefined PREFIX, selected /tmp")
    set(PREFIX "/tmp/")
endif(NOT DEFINED PREFIX)

if(PREFIX MATCHES "^/.*")
  message(WARNING " PREFIX starting with /")
  set(CMAKE_INSTALL_PREFIX "${PREFIX}")
else(PREFIX MATCHES "^/.*")
  set(PREFIX "$ENV{PWD}/${PREFIX}") 
  set(CMAKE_INSTALL_PREFIX "${PREFIX}")
endif(PREFIX MATCHES "^/.*")


if(NOT DEFINED PKG_INSTALL_PREFIX)
    set(PKG_INSTALL_PREFIX "${PREFIX}")
endif(NOT DEFINED PKG_INSTALL_PREFIX)

if( ${CMAKE_VERSION} VERSION_LESS "2.8.2" )
    if( IS_DIRECTORY "/usr/lib64" )
        set(LIBDIR "lib64")
    else( IS_DIRECTORY "/usr/lib64" )
        set(LIBDIR "lib")
    endif( IS_DIRECTORY "/usr/lib64" )
else( ${CMAKE_VERSION} VERSION_LESS "2.8.2" )
    if( IS_DIRECTORY "/usr/lib64" AND NOT IS_SYMLINK "/usr/lib64" )
        set(LIBDIR "lib64")
    else( IS_DIRECTORY "/usr/lib64" AND NOT IS_SYMLINK "/usr/lib64" )
        set(LIBDIR "lib")
    endif( IS_DIRECTORY "/usr/lib64" AND NOT IS_SYMLINK "/usr/lib64" )
endif( ${CMAKE_VERSION} VERSION_LESS "2.8.2" )

IF (UNIX)
  # since it's unix-specific we will use bash
  ADD_CUSTOM_TARGET (distclean @echo cleaning ${FASTLIB_SOURCE_DIR}
for source distribution)
  ADD_CUSTOM_TARGET (RPM @echo packaging RPMS and SRPMS)
  ADD_CUSTOM_TARGET (MOCK @echo trying to build in the mock environment)

  ADD_CUSTOM_COMMAND(TARGET MOCK
    COMMAND mock ARGS -r ${MOCKCFG} rpmbuild/SRPMS/glite-wms-api-${PVER}-${AGE}.${OS}.src.rpm
  )
  
  ADD_CUSTOM_COMMAND(TARGET RPM
    # this will create the stage area needed by rpm packager (just in case if
    # the uses didn't it already
    COMMAND make ARGS install
    COMMAND find ARGS ${PREFIX} -iname "*egg-info" -delete
    COMMAND rm ARGS -f /tmp/glite-wms-api-${PVER}-${AGE}.${OS}.tar.gz
    COMMAND ${PROJECT_SOURCE_DIR}/maketarball.sh ARGS ${PVER} ${AGE} ${OS} "${PREFIX}"
${PROJECT_SOURCE_DIR}
    COMMAND mkdir ARGS -p rpmbuild/SPECS rpmbuild/SRPMS rpmbuild/BUILD rpmbuild/RPMS rpmbuild/SOURCES
    COMMAND mv ARGS /tmp/glite-wms-api-${PVER}-${AGE}.${OS}.tar.gz rpmbuild/SOURCES/
    COMMAND sed ARGS -e 's/%{extversion}/${PVER}/g' -e 's/%{extage}/${AGE}/g'
-e 's/%{extdist}/${OS}/g' -e 's/%{extcdate}/${DATE}/g' -e 's/%{extclog}/Bug
fixing/g' < project/wms-api.spec > rpmbuild/SPECS/wms-api.spec
    COMMAND rpmbuild ARGS -ba --define "_topdir `pwd`/rpmbuild" --define
"extbuilddir ${PREFIX}" `pwd`/rpmbuild/SPECS/wms-api.spec
  )
  
ADD_CUSTOM_COMMAND(TARGET distclean
  COMMAND rm -rf wmproxy-api-cpp/src/autogen
  COMMAND make ARGS clean
  COMMAND find ARGS ${FASTLIB_SOURCE_DIR} -iname CMakeCache.txt
-delete
  COMMAND find ARGS ${FASTLIB_SOURCE_DIR} -iname cmake_install.cmake
-delete
  COMMAND find ARGS ${FASTLIB_SOURCE_DIR} -iname Makefile -delete
  COMMAND find ARGS ${FASTLIB_SOURCE_DIR} -depth -type d -iname
CMakeFiles -exec rm -rf {} \;
  COMMAND rm ARGS -rf ${PREFIX}/stage install_manifest.txt
  COMMAND rm ARGS ./pkgconfig/wms-api.pc
    VERBATIM )
ENDIF(UNIX)


set(BINDIR "bin")

set(SBINDIR "sbin")

set(LIBEXEC "libexec")

set(ENV{PKG_CONFIG_PATH}
"${PREFIX}/usr/lib/pkgconfig/:${PREFIX}/stage/usr/lib64/pkgconfig/:$ENV{PKG_CONFIG_PATH}")

if(DEFINED ADDITIONAL_PKG_CONFIG_PATH)
  set(ENV{PKG_CONFIG_PATH}
"${ADDITIONAL_PKG_CONFIG_PATH}:$ENV{PKG_CONFIG_PATH}")
endif(DEFINED ADDITIONAL_PKG_CONFIG_PATH)

set(BOOST_ROOT "/usr/lib64:/usr/lib")

find_package(PkgConfig REQUIRED)
find_package(Perl REQUIRED)
find_program(Make REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
#pkg_check_modules(GRIDSITE_GLOBUS REQUIRED gridsite-globus)
pkg_check_modules(GRIDSITE_OPENSSL REQUIRED gridsite-openssl)
pkg_check_modules(GSOAP REQUIRED gsoap)
pkg_check_modules(GSOAP_PLUSPLUS REQUIRED gsoap++)
pkg_check_modules(GSOAP_SSL REQUIRED gsoapssl)
pkg_check_modules(GSOAP_SSL_PLUSPLUS REQUIRED gsoapssl++)
pkg_check_modules(VOMS REQUIRED voms-2.0)

#FIND_PROGRAM(PYTHON_EXECUTABLE
#	NAMES python
#	PATHS /usr/bin /usr/local/bin /opt/local/bin 
#	)

#if(NOT "${PYTHON_EXECUTABLE}" STREQUAL "PYTHON_EXECUTABLE-NOTFOUND"  )
#  set(OKPYTHON "1")
#endif(NOT "${PYTHON_EXECUTABLE}" STREQUAL "PYTHON_EXECUTABLE-NOTFOUND"  )

find_file(RPMBUILD "rpmbuild"
          PATHS "/usr/bin")

find_file(CHRPATH "chrpath"
          PATHS "/usr/bin")

if( "${DEPS}" STREQUAL "yes")
if( NOT(CHRPATH AND RPMBUILD AND GRIDSITE_GLOBUS_FOUND AND GRIDSITE_OPENSSL_FOUND AND
OK_C_COMPILER AND OK_CXX_COMPILER AND VOMS_FOUND AND
GSOAP_FOUND AND
GSOAP_PLUSPLUS_FOUND AND
GSOAP_SSL_FOUND AND
GSOAP_SSL_PLUSPLUS_FOUND AND Boost_FOUND) ) 
  message( WARNING " *** Some dependencies are missing. Will try to install
them via yum. Please intert your password" )
  set(RERUN "1")
  EXEC_PROGRAM(sudo ARGS yum -y install 
boost
boost-devel
chrpath
emi-pkgconfig-compat
gridsite
gridsite-devel
gsoap
gsoap-devel
rpm-build
voms
voms-devel
gcc
gcc-c++
libstdc++
make
  )
endif( NOT(CHRPATH AND RPMBUILD AND GRIDSITE_GLOBUS_FOUND AND GRIDSITE_OPENSSL_FOUND AND
OK_C_COMPILER AND OK_CXX_COMPILER AND VOMS_FOUND AND
GSOAP_FOUND AND
GSOAP_PLUSPLUS_FOUND AND
GSOAP_SSL_FOUND AND
GSOAP_SSL_PLUSPLUS_FOUND AND Boost_FOUND) ) 
endif( "${DEPS}" STREQUAL "yes")

add_definitions( -Wall -pipe -DHAVE_STRINGSTREAM=1 -Wno-deprecated)

if(DEFINED BUILD_DBG)
  add_definitions( -g -O0 )
else(DEFINED BUILD_DBG)
  add_definitions( -O2 )
endif(DEFINED BUILD_DBG)

set(MAIN_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}")

#if(NOT DEFINED SYSCONFIGDIR)
set(SYSCONFIGDIR "${PREFIX}/etc/")
#endif(NOT DEFINED SYSCONFIGDIR)

install(FILES
        LICENSE CHANGES
        DESTINATION ${PREFIX}/usr/share/doc/glite-wms-api-${PVER}/
)

if(EXISTS "/etc/debian_version")
  set(ONDEB "1")
endif(EXISTS "/etc/debian_version")

if(EXISTS "/etc/redhat-release")
  set(ONRH "1")
endif(EXISTS "/etc/redhat-release")

if(DEFINED ONDEB)
EXEC_PROGRAM(cat
  ARGS /etc/debian_version | /usr/bin/awk -F"." '{print $1}'
  OUTPUT_VARIABLE DEB_VER)
endif(DEFINED ONDEB)

if(DEFINED ONRH)
EXEC_PROGRAM(cat
  ARGS  /etc/redhat-release | /usr/bin/awk '{print $(NF-1)}' | /usr/bin/awk
-F"." '{print $1}'
  OUTPUT_VARIABLE RH_VER)
endif(DEFINED ONRH)

if(DEFINED ONDEB)
  EXEC_PROGRAM(/usr/bin/dpkg 
    ARGS -s gsoap|grep Version|/usr/bin/awk '{print $2}'
    OUTPUT_VARIABLE GSOAP_VERSION)
endif(DEFINED ONDEB)

if(DEFINED ONRH)
    if("${RH_VER}" STREQUAL "5")
    add_definitions( -DGSOAP_VERSION_NUM=20713 )
  endif("${RH_VER}" STREQUAL "5")

  if("${RH_VER}" STREQUAL "6")
    add_definitions( -DGSOAP_VERSION_NUM=20716 )
  endif("${RH_VER}" STREQUAL "6")
endif(DEFINED ONRH)


add_subdirectory(wmproxy-api-cpp)
add_subdirectory(pkgconfig)
if(RERUN)
message(WARNING " Please re-run cmake once again...")
endif(RERUN)
