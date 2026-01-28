message(STATUS "(EC) Generating xrec Makefile")

set(EXTRA_LIBRARIES "-lXm -lXt -lSM -lICE -lXext -lX11 -lz -lm -ldl -ljpeg")

add_executable(${PROJECT_NAME} rec.f)

set(FULL_PROJECT_NAME ${PROJECT_NAME}_${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${FULL_PROJECT_NAME})

if (("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "IntelLLVM") AND (CMAKE_Fortran_COMPILER_VERSION VERSION_GREATER 2025))
  # link with libgcc_s to get a traceback
  set(GCC_S gcc_s)
endif()

target_link_libraries(${PROJECT_NAME} rec rec_util wgl x_wgl gdb gd png rmn::rmn ${GCC_S}
                      ${EXTRA_LIBRARIES} )

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
   # Symbolic link to latest build
   COMMAND ln -sfv $<TARGET_FILE_NAME:${PROJECT_NAME}> xrec
)

install(TARGETS ${PROJECT_NAME})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/xrec DESTINATION bin)
