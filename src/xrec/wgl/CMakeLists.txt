message(STATUS "Starting generation of wgl Makefile")

project(WGL C)
add_definitions(-DX_WGL)

file(GLOB CSRC  *.c)
file(GLOB FTNSRC  *.f)

add_library (wgl STATIC ${FTNSRC} ${CSRC})

