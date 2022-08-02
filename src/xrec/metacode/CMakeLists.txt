message(STATUS "(EC) Generating metacode Makefile")

add_definitions(-cpp -DUNIX -DRASTER -DC910 -DNATIF)

file(GLOB META_F_FILES  account.f asc2dsc.f blacklst.f cmpres2.f haftpal.f pushopt.f pushpar.f show.f stub.f ttputid.f ttrrini.f tttban.f tttbpr.f tttcadr.f tttcari.f
  tttcfrm.f tttclip.f tttcntr.f tttcolr.f tttcomp.f tttdraw.f tttdsbx.f ttterfi.f tttfill.f tttfini.f tttfmid.f tttfntx.f ttthaft.f tttinit.f tttline.f tttlkid.f
  tttmapc.f tttndr2.f tttoptn.f tttpati.f tttpcar.f tttprcs.f tttpre2.f tttpre3.f tttpwi1.f tttpwry.f tttrin.f tttsegm.f tttskfr.f ttttone.f tttundr.f tttv1ior.f
  tttvopn.f tttwndw.f tttxtrc.f writraw.f)

add_library (metacode STATIC ${META_F_FILES})

set(EXTRA_LIBRARIES "-lXm -lXt -lSM -lICE -lXext -lX11 -lz -lpng -lm -ldl -ljpeg")
#-lpng -lgd  -lwgl -lgd_wgl -lx_wgl -l$(LIBRMN) 

add_executable(xmetaview pmetaview.f)

add_dependencies(xmetaview metacode rec rec_util wgl gd_wgl x_wgl gdb gd png ${rmn_SRC})
target_link_libraries(xmetaview metacode rec rec_util wgl gd_wgl x_wgl wgl gdb gd png rmn ${EXTRA_LIBRARIES} )

add_executable(trames praster.f)

add_dependencies(trames metacode rec rec_util wgl gd_wgl x_wgl gdb gd png ${rmn_SRC})
target_link_libraries(trames metacode rec rec_util wgl gd_wgl x_wgl wgl gdb gd png rmn ${EXTRA_LIBRARIES} )

install(TARGETS xmetaview)
install(TARGETS trames)
