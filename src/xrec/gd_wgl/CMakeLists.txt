message(STATUS "(EC) Generating gd_wgl Makefile")

add_definitions(-DX_WGL)

file(GLOB C_FILES  *.c)

add_library (gd_wgl STATIC ${C_FILES})
target_include_directories(gd_wgl PRIVATE ${CMAKE_SOURCE_DIR}/src/xrec/include)
target_link_libraries(gd_wgl PRIVATE rmn)
