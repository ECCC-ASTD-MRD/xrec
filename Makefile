DIRS = autog png gd2 xplot88 xdash wgl x_wgl gd_wgl gmp gdb lic select selfic main convert-dict

default:	libs 

libs:
	for target in $(DIRS) ; do ( cd $$target ; make ; ) ; done

clean:
	for target in $(DIRS) metacode ; do ( cd $$target ; make clean; ) ; done

xrec:
	cd main ; make xrec

trames:
	cd metacode ; make clean_obj ; make raster ; make clean_obj
#	mv metacode/trames_$(BASE_ARCH) bin/$(BASE_ARCH)/trames

raster: trames

xmetaview:
	cd metacode ; make clean_obj ; make metaview ; make clean_obj
#	mv metacode/xmetaview_$(BASE_ARCH) bin/$(BASE_ARCH)/xmetaview

metaview: xmetaview
