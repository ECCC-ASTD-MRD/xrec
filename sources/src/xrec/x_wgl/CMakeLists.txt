message(STATUS "Starting generation of x_wgl Makefile")

project(X_WGL C)
add_definitions(-DX_WGL)

file(GLOB CSRC  *.c)
file(GLOB FTNSRC  *.f)

add_library (x_wgl STATIC ${FTNSRC} ${CSRC})

