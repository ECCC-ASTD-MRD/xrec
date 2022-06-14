message(STATUS "(EC) Generating gd Makefile")

add_definitions(-DHAVE_LIBPNG)

file(GLOB C_FILES  *.c)

add_library (gd STATIC ${C_FILES})

