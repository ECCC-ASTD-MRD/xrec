message(STATUS "(EC) Generating xrec Makefile")

set(EXTRA_LIBRARIES "-lXm -lXt -lSM -lICE -lXext -lX11 -lz -lm -ldl -ljpeg")

add_executable(xrec rec.f)

add_dependencies(xrec rec rec_util wgl gd_wgl x_wgl gdb gd png rmn)
target_link_libraries(xrec rec rec_util wgl gd_wgl x_wgl gdb gd png rmn
                      ${EXTRA_LIBRARIES} )

install(TARGETS xrec RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
