message(STATUS "(EC) Generating rec Makefile")

add_definitions(-DX_WGL -DMONOCHROME)

file(GLOB C_FILES  */*.c)
file(GLOB F_FILES  */*.f */*.f90)

# Utiliser un niveau d'optimisation plus bas pour ce fichier, car Intel 2025 semblant optimiser agressivement une boucle
set_source_files_properties(main/contour.c PROPERTIES COMPILE_FLAGS "$<$<COMPILE_LANG_AND_ID:C,IntelLLVM>:-O1>")

add_library (rec STATIC ${F_FILES} ${C_FILES})
add_dependencies(rec ${PROJECT_NAME}_build_info)
target_include_directories(rec PRIVATE ${CMAKE_SOURCE_DIR}/src/xrec/include)
target_link_libraries(rec PRIVATE rmn::rmn)
