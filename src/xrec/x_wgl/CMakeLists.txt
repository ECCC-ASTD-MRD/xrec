message(STATUS "(EC) Generating x_wgl Makefile")

add_definitions(-DX_WGL)

file(GLOB C_FILES  *.c)
file(GLOB F_FILES  *.f)

add_library (x_wgl STATIC ${F_FILES} ${C_FILES})
target_include_directories(x_wgl PRIVATE ${CMAKE_SOURCE_DIR}/src/xrec/include)

