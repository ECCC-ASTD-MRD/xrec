message(STATUS "Starting generation of gd Makefile")

project(GD C)
add_definitions(-DHAVE_LIBPNG)

file(GLOB CSRC  *.c)

add_library (gd STATIC ${CSRC})

