message(STATUS "Starting generation of gdb Makefile")

project(GDB C)
add_definitions(-DX_WGL -DLinux -DSOCKADDR)

file(GLOB CSRC  *.c)

add_library (gdb STATIC ${CSRC})

