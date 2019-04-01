message(STATUS "Starting generation of gd_wgl Makefile")

project(GD_WGL C)
add_definitions(-DX_WGL)

file(GLOB CSRC  *.c)

add_library (gd_wgl STATIC ${CSRC})

