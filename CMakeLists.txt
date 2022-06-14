cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_rpn)

include(ec_init)           # Include EC specific cmake utils
ec_git_version()           # Get the version from the git repository
ec_parse_manifest()        # Parse MANIFEST file

project(${NAME} DESCRIPTION "${DESCRIPTION}")
set(PROJECT_VERSION ${VERSION}${STATE})
set(XOAS_VERSION ${PROJECT_VERSION} CACHE INTERNAL "XOAS version" FORCE)
message(STATUS "(EC) XOAS VERSION = ${XOAS_VERSION}")

enable_language(Fortran C)

ec_build_info()            # Generate build include file
include(ec_compiler_presets)

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

# Include an architecture dependent file that contains include files for a
# particular architecture and compiler, using the default value of the
# compiler or the argument given to cmake command.
message(STATUS "(EC) arch-dependent include directory: ${CMAKE_SOURCE_DIR}/src/archdep/include/${BUILD_ARCH}-${COMPILER_SUITE}")
include_directories(
   ${CMAKE_SOURCE_DIR}/src/archdep/include/${BUILD_ARCH}-${COMPILER_SUITE}
   ${CMAKE_SOURCE_DIR}/src/rmn/PUBLIC_INCLUDES
   ${CMAKE_SOURCE_DIR}/src/RPN/compiler_includes
   )

if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/work-${OS}-${COMPILER_SUITE}-${COMPILER_VERSION}" CACHE FILEPATH "CMake Installation prefix" FORCE)
endif()

add_subdirectory(src src)

install(FILES ${CMAKE_SOURCE_DIR}/scripts/setup-xrec.dot DESTINATION ${CMAKE_INSTALL_PREFIX})
ec_build_config()      # Create build configuration script
