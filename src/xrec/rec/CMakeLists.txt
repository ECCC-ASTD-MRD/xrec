message(STATUS "(EC) Generating rec Makefile")

add_definitions(-DX_WGL -DMONOCHROME)

file(GLOB C_FILES  */*.c)
file(GLOB F_FILES  */*.f */*.f90)

add_library (rec STATIC ${F_FILES} ${C_FILES})
add_dependencies(rec ${PROJECT_NAME}_build_info)
target_include_directories(rec PRIVATE ${CMAKE_SOURCE_DIR}/src/xrec/include)
