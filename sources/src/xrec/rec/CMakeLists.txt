message(STATUS "Starting generation of rec Makefile")

project(REC C)
add_definitions(-DX_WGL -DMONOCHROME)

file(GLOB CSRC  */*.c)
file(GLOB FTN_SOURCES  */*.f */*.f90)

add_library (rec STATIC ${FTN_SOURCES} ${CSRC})

