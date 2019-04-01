message(STATUS "Starting generation of png Makefile")

project(PNG C)

file(GLOB CSRC  *.c)

add_library (png STATIC ${CSRC})

