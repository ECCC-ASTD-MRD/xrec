cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_rpn)

include(ec_init)           # Include EC specific cmake utils
ec_git_version()           # Get the version from the git repository
ec_parse_manifest()        # Parse MANIFEST file

project(${NAME} DESCRIPTION "${DESCRIPTION}")
set(PROJECT_VERSION ${VERSION}${STATE})
set(xrec_VERSION ${PROJECT_VERSION} CACHE INTERNAL "xrec version" FORCE)
message(STATUS "(EC) xrec VERSION = ${xrec_VERSION}")

enable_language(Fortran C)

ec_build_info()            # Generate build include file
include(ec_compiler_presets)

set(WITH_OPENMP FALSE CACHE BOOL "Control whether to use OpenMP")
add_compile_definitions(WITHOUT_OpenMP)

# CMAKE_BUILD_TYPE can be one of Debug, Release, RelWithDebInfo, MinSizeRel
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "(EC) CMAKE_BUILD_TYPE can be one of Debug, Release, RelWithDebInfo, MinSizeRel")
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "(EC) No build type selected, default to ${CMAKE_BUILD_TYPE}")
endif()

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

if (EXISTS ${CMAKE_SOURCE_DIR}/src/rmn/CMakeLists.txt)
  message(STATUS "(EC) Using librmn sources")
  set(rmn_SRC "rmn" CACHE STRING "rmn sources")
  set(rmn_FOUND TRUE CACHE BOOL "Control whether rmn library was found")
  set(rmn_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/rmn/include" CACHE PATH "RMN include directory" FORCE)
  set(rmn_PUBLIC_INCLUDES_DIR "${CMAKE_SOURCE_DIR}/src/rmn/src/PUBLIC_INCLUDES" CACHE PATH "RMN public includes directory" FORCE)
  include_directories(${rmn_INCLUDE_DIR} ${rmn_PUBLIC_INCLUDES_DIR})
  add_subdirectory(src/rmn rmn)
else()
  find_package(rmn ${rmn_REQ_VERSION} CONFIG)
endif()
  
add_subdirectory(src/xrec xrec)

add_custom_target(work
  COMMAND /bin/mkdir -p ${WORK_PREFIX}
  COMMAND /bin/mkdir -p ${WORK_PREFIX}/bin
  COMMAND /bin/cp ${CMAKE_BINARY_DIR}/xrec-config ${WORK_PREFIX}/bin
  COMMAND cd ${CMAKE_BINARY_DIR}/xrec/main && make install/local
  COMMAND cp -p ${CMAKE_SOURCE_DIR}/scripts/setup-xrec.dot ${WORK_PREFIX}
)

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
