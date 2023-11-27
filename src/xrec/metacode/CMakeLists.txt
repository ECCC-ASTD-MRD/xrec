message(STATUS "(EC) Generating metacode Makefile")

add_definitions(-cpp)

file(GLOB F_FILES  account.f asc2dsc.f blacklst.f cmpres2.f haftpal.f pushopt.f pushpar.f show.f stub.f ttputid.f ttrrini.f tttban.f tttbpr.f tttcadr.f tttcari.f
  tttcfrm.f tttclip.f tttcntr.f tttcolr.f tttcomp.f tttdraw.f tttdsbx.f ttterfi.f tttfill.f tttfini.f tttfmid.f tttfntx.f ttthaft.f tttinit.f tttline.f tttlkid.f
  tttmapc.f tttndr2.f tttoptn.f tttpati.f tttpcar.f tttprcs.f tttpre2.f tttpre3.f tttpwi1.f tttpwry.f tttrin.f tttsegm.f tttskfr.f ttttone.f tttundr.f tttv1ior.f
  tttvopn.f tttwndw.f tttxtrc.f writraw.f)

message(STATUS "(EC) trames VERSION = ${TRAMES_VERSION}")
message(STATUS "(EC) xmetaview VERSION = ${XMETAVIEW_VERSION}")

# We need to create two libraries, one with -DNATIF and another without that definition
add_library(metacode-metaview STATIC ${F_FILES})
target_compile_definitions(metacode-metaview PUBLIC UNIX RASTER C910 NATIF)

add_library(metacode-trames STATIC ${F_FILES})
target_compile_definitions(metacode-trames PUBLIC UNIX RASTER C910)

set(EXTRA_LIBRARIES "-lXm -lXt -lSM -lICE -lXext -lX11 -lz -lpng -lm -ldl -ljpeg")

add_executable(trames praster.f)
set(FULL_TRAMES_NAME trames_${TRAMES_VERSION})
set_target_properties(trames PROPERTIES 
   OUTPUT_NAME ${FULL_TRAMES_NAME}
   COMPILE_DEFINITIONS TRAMES_VERSION="${TRAMES_VERSION}")
add_dependencies(trames metacode-trames rec rec_util wgl gd_wgl x_wgl gdb gd png)
target_link_libraries(trames metacode-trames rec rec_util wgl gd_wgl x_wgl wgl gdb gd png rmn::rmn ${EXTRA_LIBRARIES} )
target_compile_definitions(trames PUBLIC UNIX RASTER C910)

add_executable(xmetaview pmetaview.f)
set(FULL_XMETAVIEW_NAME xmetaview_${XMETAVIEW_VERSION})
set_target_properties(xmetaview PROPERTIES
  OUTPUT_NAME ${FULL_XMETAVIEW_NAME}
  COMPILE_DEFINITIONS XMETAVIEW_VERSION="${XMETAVIEW_VERSION}")
add_dependencies(xmetaview metacode-metaview rec rec_util wgl gd_wgl x_wgl gdb gd png)
target_link_libraries(xmetaview metacode-metaview rec rec_util wgl gd_wgl x_wgl wgl gdb gd png rmn::rmn ${EXTRA_LIBRARIES} )
target_compile_definitions(xmetaview PUBLIC UNIX RASTER C910 NATIF)

add_custom_command(TARGET trames POST_BUILD
   # Symbolic links to latest build
   COMMAND ln -sfv $<TARGET_FILE_NAME:trames> trames
   COMMAND ln -sfv $<TARGET_FILE_NAME:xmetaview> xmetaview
   )

install(TARGETS trames)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/trames DESTINATION bin)
install(TARGETS xmetaview)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/xmetaview DESTINATION bin)
