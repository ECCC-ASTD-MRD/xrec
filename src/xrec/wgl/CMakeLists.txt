message(STATUS "(EC) Generating wgl Makefile")

add_definitions(-DX_WGL)

file(GLOB C_FILES  *.c)
file(GLOB F_FILES  *.f)

add_library (wgl STATIC ${F_FILES} ${C_FILES})
target_include_directories(wgl PRIVATE ${CMAKE_SOURCE_DIR}/src/xrec/include)
target_link_libraries(wgl PRIVATE rmn::rmn)

