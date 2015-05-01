#DIRS = autog png gd2 xplot88 xdash wgl x_wgl gd_wgl gmp gdb lic select selfic main convert-dict xinit
DIRS = autog png gd2 xplot88 xdash wgl x_wgl gd_wgl gmp gdb lic select selfic main x_wgl

include metacode/version.inc
include main/version.inc

LIBRMN = rmn_015.2

default:	libs 

all:	libs xrec trames xmetaview

libs:
	for target in $(DIRS) ; do ( cd $$target ; make ; ) ; done

clean:
	for target in $(DIRS) metacode ; do ( cd $$target ; make clean; ) ; done

xrec:   libs
	mkdir -p bin/$(BASE_ARCH)
	cd main ; make LIBRMN=$(LIBRMN) xrec
	mv main/xrec_$(subst ",,$(XREC_VERSION))_$(BASE_ARCH) bin/$(BASE_ARCH)/xrec

trames:
	mkdir -p bin/$(BASE_ARCH)
	cd metacode ; make clean_obj ; make LIBRMN=$(LIBRMN) raster ; make clean_obj
	mv metacode/trames_$(subst ",,$(TRAMES_VERSION))_$(BASE_ARCH) bin/$(BASE_ARCH)/trames

raster: trames

xmetaview: libs
	mkdir -p bin/$(BASE_ARCH)
	cd metacode ; make clean_obj ; make LIBRMN=$(LIBRMN) metaview ; make clean_obj
	mv metacode/xmetaview_$(subst ",,$(XMETAVIEW_VERSION))_$(BASE_ARCH) bin/$(BASE_ARCH)/xmetaview

metaview: xmetaview
