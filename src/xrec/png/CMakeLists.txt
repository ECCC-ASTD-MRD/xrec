message(STATUS "(EC) Generating png Makefile")

file(GLOB C_FILES  *.c)

add_library (png STATIC ${C_FILES})

