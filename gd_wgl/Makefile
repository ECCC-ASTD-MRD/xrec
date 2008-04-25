include /usr/local/env/armnlib/include/$(ARCH)$(ABI)/Makefile_addons

.SUFFIXES : 

.SUFFIXES : .ftn .c .o  .a

SHELL = /bin/sh

CPP = /lib/cpp

GD_WGLLIB = ../lib/$(EC_ARCH)$(ABI)/libgd_wgl.a

CFLAGS =  -I/usr/X11R6/include -I$(ARMNLIB)/include -I../include -I$(HOME)/src/gd2/ -DX_WGL

OPTIMIZ = -debug -O 0
OPTIMIZ = -O 3

CPPFLAGS = -I$(ARMNLIB)/include =-I../include -DX_WGL

.PRECIOUS:

default: gd_wgllib

.c.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.ftn.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.a:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.o

.ftn.a:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.o

CDECKS= \
gd_pwrit.c\
gd_wglafi.c\
gd_wglafx.c\
gd_wglali.c\
gd_wglalx.c\
gd_wglbbf.c\
gd_wglbti.c\
gd_wglbtn.c\
gd_wglbtp.c\
gd_wglbtpi.c\
gd_wglbtpx.c\
gd_wglbtx.c\
gd_wglcfi.c\
gd_wglcfx.c\
gd_wglchngfs.c\
gd_wglcli.c\
gd_wglclr.c\
gd_wglclw.c\
gd_wglclx.c\
gd_wglcmi.c\
gd_wglcmx.c\
gd_wglcol.c\
gd_wglcolf.c\
gd_wgldbf.c\
gd_wgldcm.c \
gd_wgldeflst.c\
gd_wgldpt.c\
gd_wgldri.c\
gd_wglecr.c\
gd_wglfbf.c\
gd_wglfsh.c\
gd_wglfshlb.c\
gd_wglgvx.c\
gd_wglgpl.c\
gd_wglgwz.c\
gd_wgliax.c\
gd_wglinicmap.c\
gd_wglinids.c\
gd_wglinipat.c\
gd_wglinit.c\
gd_wgliniwin.c\
gd_wgliniwpx.c\
gd_wglinstcmap.c\
gd_wgllwi.c\
gd_wglmapc.c\
gd_wglmapcs.c\
gd_wglmco.c\
gd_wglmcos.c\
gd_wglmcw.c\
gd_wglmvi.c\
gd_wglmesagr.c\
gd_wglncl.c\
gd_wglnewwin.c\
gd_wglopw.c\
gd_wglpfi.c\
gd_wglpfx.c\
gd_wglpli.c\
gd_wglplx.c\
gd_wglppo.c\
gd_wglpsz.c\
gd_wglpti.c\
gd_wglptis.c\
gd_wglptxs.c\
gd_wglrfi.c\
gd_wglrli.c\
gd_wglrfx.c\
gd_wglroc.c\
gd_wglrwc.c\
gd_wglsbf.c\
gd_wglsetw.c\
gd_wglsetwcmap.c\
gd_wglsld.c\
gd_wglsmk.c\
gd_wglspt.c\
gd_wglssp.c\
gd_wglswb.c\
gd_wglwtwrsz.c\
gd_wglxai.c

obj: $(CDECKS) 
#Produire les fichiers objets (.o) pour tous les fichiers

qclean:
#Faire un petit menage. On enleve tous les .o et les .f \nproduits a partir de .ftn/.ptn
	-if [ "*.[fp]tn" != "`echo *.[fp]tn`" ] ; \
	then \
	for i in *.[fp]tn ; \
	do \
	fn=`r.basename $$i '.[fp]tn'`; \
	rm -f $$fn.f; \
	done \
	fi
	rm *.o

gd_wgllib:	$(GD_WGLLIB)(gd_pwrit.o)\
$(GD_WGLLIB)(gd_wglafi.o)\
$(GD_WGLLIB)(gd_wglafx.o)\
$(GD_WGLLIB)(gd_wglali.o)\
$(GD_WGLLIB)(gd_wglalx.o)\
$(GD_WGLLIB)(gd_wglbbf.o)\
$(GD_WGLLIB)(gd_wglbti.o)\
$(GD_WGLLIB)(gd_wglbtn.o)\
$(GD_WGLLIB)(gd_wglbtp.o)\
$(GD_WGLLIB)(gd_wglbtpi.o)\
$(GD_WGLLIB)(gd_wglbtpx.o)\
$(GD_WGLLIB)(gd_wglbtx.o)\
$(GD_WGLLIB)(gd_wglcfi.o)\
$(GD_WGLLIB)(gd_wglcfx.o)\
$(GD_WGLLIB)(gd_wglchngfs.o)\
$(GD_WGLLIB)(gd_wglcli.o)\
$(GD_WGLLIB)(gd_wglclr.o)\
$(GD_WGLLIB)(gd_wglclw.o)\
$(GD_WGLLIB)(gd_wglclx.o)\
$(GD_WGLLIB)(gd_wglcmi.o)\
$(GD_WGLLIB)(gd_wglcmx.o)\
$(GD_WGLLIB)(gd_wglcol.o)\
$(GD_WGLLIB)(gd_wglcolf.o)\
$(GD_WGLLIB)(gd_wgldbf.o)\
$(GD_WGLLIB)(gd_wgldcm.o)\
$(GD_WGLLIB)(gd_wgldeflst.o)\
$(GD_WGLLIB)(gd_wgldpt.o)\
$(GD_WGLLIB)(gd_wgldri.o)\
$(GD_WGLLIB)(gd_wglecr.o)\
$(GD_WGLLIB)(gd_wglfbf.o)\
$(GD_WGLLIB)(gd_wglfsh.o)\
$(GD_WGLLIB)(gd_wglfshlb.o)\
$(GD_WGLLIB)(gd_wglgpl.o)\
$(GD_WGLLIB)(gd_wglgvx.o)\
$(GD_WGLLIB)(gd_wglgwz.o)\
$(GD_WGLLIB)(gd_wgliax.o)\
$(GD_WGLLIB)(gd_wglinicmap.o)\
$(GD_WGLLIB)(gd_wglinids.o)\
$(GD_WGLLIB)(gd_wglinipat.o)\
$(GD_WGLLIB)(gd_wglinit.o)\
$(GD_WGLLIB)(gd_wgliniwin.o)\
$(GD_WGLLIB)(gd_wgliniwpx.o)\
$(GD_WGLLIB)(gd_wglinstcmap.o)\
$(GD_WGLLIB)(gd_wgllwi.o)\
$(GD_WGLLIB)(gd_wglmapc.o)\
$(GD_WGLLIB)(gd_wglmapcs.o)\
$(GD_WGLLIB)(gd_wglmco.o)\
$(GD_WGLLIB)(gd_wglmcos.o)\
$(GD_WGLLIB)(gd_wglmcw.o)\
$(GD_WGLLIB)(gd_wglmesagr.o)\
$(GD_WGLLIB)(gd_wglmvi.o)\
$(GD_WGLLIB)(gd_wglncl.o)\
$(GD_WGLLIB)(gd_wglnewwin.o)\
$(GD_WGLLIB)(gd_wglopw.o)\
$(GD_WGLLIB)(gd_wglpfi.o)\
$(GD_WGLLIB)(gd_wglpfx.o)\
$(GD_WGLLIB)(gd_wglpli.o)\
$(GD_WGLLIB)(gd_wglplx.o)\
$(GD_WGLLIB)(gd_wglppo.o)\
$(GD_WGLLIB)(gd_wglpsz.o)\
$(GD_WGLLIB)(gd_wglpti.o)\
$(GD_WGLLIB)(gd_wglptis.o)\
$(GD_WGLLIB)(gd_wglptxs.o)\
$(GD_WGLLIB)(gd_wglrfi.o)\
$(GD_WGLLIB)(gd_wglrli.o)\
$(GD_WGLLIB)(gd_wglrfx.o)\
$(GD_WGLLIB)(gd_wglroc.o)\
$(GD_WGLLIB)(gd_wglrwc.o)\
$(GD_WGLLIB)(gd_wglsbf.o)\
$(GD_WGLLIB)(gd_wglsetw.o)\
$(GD_WGLLIB)(gd_wglsetwcmap.o)\
$(GD_WGLLIB)(gd_wglsld.o)\
$(GD_WGLLIB)(gd_wglsmk.o)\
$(GD_WGLLIB)(gd_wglspt.o)\
$(GD_WGLLIB)(gd_wglssp.o)\
$(GD_WGLLIB)(gd_wglswb.o)\
$(GD_WGLLIB)(gd_wglwtwrsz.o)\
$(GD_WGLLIB)(gd_wglxai.o)            
