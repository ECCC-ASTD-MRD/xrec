message(STATUS "(EC) Generating gd_wgl Makefile")

add_definitions(-DX_WGL)

file(GLOB C_FILES  *.c)

add_library (gd_wgl STATIC ${C_FILES})

