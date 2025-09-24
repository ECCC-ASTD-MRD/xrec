cmake_minimum_required(VERSION 3.16)

#----- Append EC specific module path
list(APPEND CMAKE_MODULE_PATH $ENV{EC_CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake_rpn/modules)

include(ec_init)           # Include EC specific cmake utils
ec_git_version()           # Get the version from the git repository
ec_parse_manifest()        # Parse MANIFEST file

project(${NAME} DESCRIPTION "${DESCRIPTION}" LANGUAGES C Fortran)
set(PROJECT_VERSION ${VERSION}${STATE})
set(${NAME}_VERSION ${PROJECT_VERSION} CACHE INTERNAL "xrec version" FORCE)
message(STATUS "(EC) xrec VERSION = ${xrec_VERSION}")

ec_build_info()            # Generate build include file

include(ec_compiler_presets)

set(WITH_OPENMP FALSE CACHE BOOL "Control whether to use OpenMP")
include(ec_openmp)

# Figure out the correct arch-dependent include path
set(BUILD_ARCH "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "(EC) Build architecture: ${BUILD_ARCH}")
message(STATUS "(EC) Sources directory is: ${CMAKE_SOURCE_DIR}")
message(STATUS "(EC) Build directory is: ${CMAKE_BINARY_DIR}")

# get name and version of operating system
execute_process(COMMAND sh "-c" "${CMAKE_SOURCE_DIR}/scripts/os.sh" OUTPUT_VARIABLE OS)
message(STATUS "(EC) Operating system is: ${OS}")

# get name and version of compiler
if(DEFINED ENV{COMPILER_VERSION})
   set(COMPILER_VERSION $ENV{COMPILER_VERSION})
else()
  execute_process(COMMAND sh "-c" "${CMAKE_SOURCE_DIR}/scripts/compiler.sh ${COMPILER_SUITE}" OUTPUT_VARIABLE COMPILER_VERSION)
endif()
message(STATUS "(EC) Compiler version: ${COMPILER_VERSION}")

set(WORK_PREFIX "${CMAKE_SOURCE_DIR}/work-${OS}-${COMPILER_SUITE}-${COMPILER_VERSION}" CACHE FILEPATH "Working directory prefix" FORCE)
message(STATUS "(EC) Setting working directory prefix to ${WORK_PREFIX}")

if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
  set(CMAKE_INSTALL_PREFIX "${WORK_PREFIX}" CACHE FILEPATH "CMake Installation prefix" FORCE)
endif()

if(NOT DEFINED ENV{EC_ARCH})
  if (EXISTS ${CMAKE_SOURCE_DIR}/src/rmn/CMakeLists.txt)
    message(STATUS "(EC) Using librmn sources")
    set(rmn_FOUND TRUE CACHE BOOL "Control whether rmn library was found")
    add_subdirectory(src/rmn rmn)
   else()
     message(FATAL_ERROR "(EC) No rmn directory found! Please update git submodule")
  endif()
else()
  find_package(rmn ${rmn_REQ_VERSION} COMPONENTS static REQUIRED)
endif()
  
add_subdirectory(src/xrec xrec)

add_custom_target(work
  COMMAND /bin/mkdir -p ${WORK_PREFIX}
  COMMAND /bin/mkdir -p ${WORK_PREFIX}/bin
  COMMAND /bin/cp ${CMAKE_BINARY_DIR}/xrec-config ${WORK_PREFIX}/bin
  COMMAND cd ${CMAKE_BINARY_DIR}/xrec/main && make install/local
  COMMAND cp -p ${CMAKE_SOURCE_DIR}/scripts/setup-xrec.dot ${WORK_PREFIX}
)

include(CTest)
add_custom_target(check COMMAND CTEST_OUTPUT_ON_FAILURE=true ${CMAKE_CTEST_COMMAND})
add_test(NAME test-trames COMMAND ${BASH_COMMAND} ${CMAKE_BINARY_DIR}/xrec/metacode/trames -mc ${CMAKE_SOURCE_DIR}/src/xrec/metacode/metacod)

#----- Packaging
ec_package_name()    # Define package prefix
ec_build_config()    # Create build configuration script
ec_prepare_ssm()     # Prepare ssm packaging files

set(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGE_VENDOR "ECCC")
set(CPACK_PACKAGE_CONTACT "${MAINTAINER}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/package")
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CPACK_PACKAGE_FILE_NAME "${PACKAGE_NAME}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${NAME}_${PROJECT_VERSION}")
include(CPack)

