message(STATUS "(EC) Generating gd Makefile")

add_definitions(-DHAVE_LIBPNG)

file(GLOB C_FILES  *.c)

add_library (gd STATIC ${C_FILES})
target_include_directories(gd PRIVATE ${CMAKE_SOURCE_DIR}/src/xrec/include)

