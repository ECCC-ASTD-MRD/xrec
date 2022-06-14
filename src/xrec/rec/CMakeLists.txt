message(STATUS "(EC) Generating rec Makefile")

add_definitions(-DX_WGL -DMONOCHROME)

file(GLOB C_FILES  */*.c)
file(GLOB F_FILES  */*.f */*.f90)

add_library (rec STATIC ${F_FILES} ${C_FILES})
