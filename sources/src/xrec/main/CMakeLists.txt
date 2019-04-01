message(STATUS "Starting generation of xrec Makefile")

project(XREC Fortran)
set(EXTRA_LIBRARIES "-lXm -lXt -lSM -lICE -lXext -lX11 -lz -lm -ldl -ljpeg")
#set(EXTRA_LIBRARIES "-lXm -lXt -lSM -lICE -lXext -lXp -lX11 -lz -lm -ldl -ljpeg")

add_executable(xrec rec.f)
#add_dependencies(xrec rec rec_util wgl gd_wgl x_wgl gdb gd png)
add_dependencies(xrec rec rec_util wgl gd_wgl x_wgl gdb gd png rmn)
target_link_libraries(xrec rec rec_util wgl gd_wgl x_wgl gdb gd png rmn
                      ${Fortran_LIBRARIES}
                      ${LAPACK_LIBRARIES} ${BLAS_LIBRARIES} ${EXTRA_LIBRARIES} )
#                     -lrmn)

install(TARGETS xrec RUNTIME
        DESTINATION ${ROOT}/../work/bin-${OS}-${COMPILER}-${COMP_VERSION}/)
install(FILES ${ROOT}/README.run
        DESTINATION ${ROOT}/../work/bin-${OS}-${COMPILER}-${COMP_VERSION} 
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
