message(STATUS "(EC) Generating gdb Makefile")

add_definitions(-DX_WGL -DLinux -DSOCKADDR)

file(GLOB C_FILES  *.c)

add_library (gdb STATIC ${C_FILES})
target_include_directories(gdb PRIVATE ${CMAKE_SOURCE_DIR}/src/xrec/include)

