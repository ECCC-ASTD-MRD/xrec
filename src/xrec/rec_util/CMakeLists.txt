message(STATUS "Starting generation of rec_util Makefile")

project(REC_UTIL C)
add_definitions(-DX_WGL -DX_REC_UTIL)

file(GLOB CSRC  */*.c)
file(GLOB FTNSRC */*.f)

add_library (rec_util STATIC ${FTNSRC} ${CSRC})

