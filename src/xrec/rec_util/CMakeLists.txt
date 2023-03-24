message(STATUS "(EC) Generating rec_util Makefile")

add_definitions(-DX_WGL -DX_REC_UTIL)

file(GLOB C_FILES  */*.c)
file(GLOB F_FILES */*.f)

add_library (rec_util STATIC ${F_FILES} ${C_FILES})
target_include_directories(rec_util PRIVATE ${CMAKE_SOURCE_DIR}/src/xrec/include)
target_link_libraries(rec_util PRIVATE rmn::rmn)
