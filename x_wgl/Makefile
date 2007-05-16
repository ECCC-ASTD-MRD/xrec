include /usr/local/env/armnlib/include/$(ARCH)$(ABI)/Makefile_addons

.SUFFIXES : 

.SUFFIXES : .ftn .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

X_WGLLIB = $(HOME)/userlibs/$(ARCH)$(ABI)/libx_wgl.a

FFLAGS = 

CFLAGS =  -I/usr/X11R6/include -I$(ARMNLIB)/include -I../include -DX_WGL  -I$(HOME)/src/gd/

OPTIMIZ = -debug -O 0
OPTIMIZ = -O 3

CPPFLAGS = -I$(ARMNLIB)/include =-I../include -DX_WGL

.PRECIOUS:

default: x_wgllib

.ftn.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.c.a:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.o

.ftn.a:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.o

CDECKS= \
	x_bitfinder.c  x_initdev.c	\
	x_souris2.c flush.c \
	x_wglafi.c     x_wglali.c	\
	x_wglasi.c     x_wglbbf.c	x_wglbrb.c \
	x_wglbti.c     x_wglbtn.c	 x_wglbtp.c	x_wglbtpi.c \
	x_wglcfi.c	\
	x_wglchngfs.c  x_wglcli.c	 x_wglclr.c	x_wglclw.c \
	x_wglcmi.c	 x_wglcol.c \
	x_wglcolf.c    x_wglcrb.c	 x_wgldbf.c	x_wgldbg.c \
	x_wgldcm.c     x_wgldeflst.c	 x_wgldefncw.c	x_wgldld.c \
	x_wgldpt.c     x_wgldri.c	 x_wgldsi.c x_wglecr.c \
	x_wglerb.c	 x_wglfbf.c	x_wglfsc.c \
	x_wglfsh.c     x_wglfshlb.c	 x_wglfsz.c	x_wglgacw.c \
	x_wglgaf.c     x_wglgbf.c	 x_wglgci.c	x_wglgco.c \
	x_wglgdbg.c    x_wglgetcmap.c	 x_wglgld.c	x_wglglpti.c \
	x_wglglw.c     x_wglgmk.c	 x_wglgmod.c	x_wglgpl.c \
	x_wglgpt.c     x_wglgsi.c	 x_wglgsp.c	\
	x_wglgvi.c     x_wglgvp.c	 x_wglgwz.c \
	x_wglgzi.c     x_wglgzp.c	 x_wglhsi.c \
	x_wglias.c	 x_wglinicmap.c \
	x_wglinids.c   x_wglinipat.c	 x_wglinit.c	x_wgliniwin.c \
	x_wgliniwpx.c  x_wglinstcmap.c	 x_wglinvpat.c	x_wglkas.c \
	x_wgllpt.c     x_wgllwi.c	 x_wglmapc.c	x_wglmapcs.c \
	x_wglmco.c     x_wglmcos.c	 x_wglmcw.c	x_wglmesagr.c \
	x_wglmvi.c     x_wglncl.c	x_wglnewwin.c \
	x_wglopmw.c    x_wglopw.c	 x_wglpfi.c	\
	x_wglpli.c     x_wglppo.c	x_wglpsi.c \
	x_wglpsz.c	 x_wglpti.c	x_wglptis.c \
	x_wglptxs.c	 x_wglrfi.c	\
	x_wglrli.c     x_wglroc.c	x_wglrstdpar.c \
	x_wglrwc.c     x_wglsavpfc.c	 x_wglsbf.c	\
	x_wglsetw.c    x_wglsetwcmap.c	 x_wglsld.c	x_wglsmk.c \
	x_wglspt.c     x_wglstu.c	x_wglswb.c \
	x_wglwsi.c     x_wglwtwrsz.c	x_wglxai.c \
	x_xmmenu.c     x_wglgzx.c	x_invertChar.c  \
	x_wwwlpt.c


obj: $(CDECKS) $(FDECKS) 
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

x_wgllib: $(X_WGLLIB)(x_bitfinder.o) \
	$(X_WGLLIB)(flush.o) \
	$(X_WGLLIB)(x_initdev.o) \
	$(X_WGLLIB)(x_wglafi.o) \
	$(X_WGLLIB)(x_wglali.o) \
	$(X_WGLLIB)(x_wglasi.o) \
	$(X_WGLLIB)(x_wglbbf.o) \
	$(X_WGLLIB)(x_wglbrb.o) \
	$(X_WGLLIB)(x_wglbti.o) \
	$(X_WGLLIB)(x_wglbtn.o) \
	$(X_WGLLIB)(x_wglbtp.o) \
	$(X_WGLLIB)(x_wglbtpi.o) \
	$(X_WGLLIB)(x_wglcfi.o) \
	$(X_WGLLIB)(x_wglchngfs.o) \
	$(X_WGLLIB)(x_wglcli.o) \
	$(X_WGLLIB)(x_wglclr.o) \
	$(X_WGLLIB)(x_wglclw.o) \
	$(X_WGLLIB)(x_wglcmi.o) \
	$(X_WGLLIB)(x_wglcol.o) \
	$(X_WGLLIB)(x_wglcolf.o) \
	$(X_WGLLIB)(x_wglcrb.o) \
	$(X_WGLLIB)(x_wgldbf.o) \
	$(X_WGLLIB)(x_wgldbg.o) \
	$(X_WGLLIB)(x_wgldcm.o) \
	$(X_WGLLIB)(x_wgldeflst.o) \
	$(X_WGLLIB)(x_wgldefncw.o) \
	$(X_WGLLIB)(x_wgldld.o) \
	$(X_WGLLIB)(x_wgldpt.o) \
	$(X_WGLLIB)(x_wgldri.o) \
	$(X_WGLLIB)(x_wgldsi.o) \
	$(X_WGLLIB)(x_wglecr.o) \
	$(X_WGLLIB)(x_wglerb.o) \
	$(X_WGLLIB)(x_wglfbf.o) \
	$(X_WGLLIB)(x_wglfsc.o) \
	$(X_WGLLIB)(x_wglfsh.o) \
	$(X_WGLLIB)(x_wglfshlb.o) \
	$(X_WGLLIB)(x_wglfsz.o) \
	$(X_WGLLIB)(x_wglgacw.o) \
	$(X_WGLLIB)(x_wglgaf.o) \
	$(X_WGLLIB)(x_wglgbf.o) \
	$(X_WGLLIB)(x_wglgci.o) \
	$(X_WGLLIB)(x_wglgco.o) \
	$(X_WGLLIB)(x_wglgdbg.o) \
	$(X_WGLLIB)(x_wglgetcmap.o) \
	$(X_WGLLIB)(x_wglgld.o) \
	$(X_WGLLIB)(x_wglglpti.o) \
	$(X_WGLLIB)(x_wglglw.o) \
	$(X_WGLLIB)(x_wglgmk.o) \
	$(X_WGLLIB)(x_wglgmod.o) \
	$(X_WGLLIB)(x_wglgpl.o) \
	$(X_WGLLIB)(x_wglgpt.o) \
	$(X_WGLLIB)(x_wglgsi.o) \
	$(X_WGLLIB)(x_wglgsp.o) \
	$(X_WGLLIB)(x_wglgvi.o) \
	$(X_WGLLIB)(x_wglgvp.o) \
	$(X_WGLLIB)(x_wglgwz.o) \
	$(X_WGLLIB)(x_wglgzi.o) \
	$(X_WGLLIB)(x_wglgzp.o) \
	$(X_WGLLIB)(x_wglgzx.o) \
	$(X_WGLLIB)(x_wglhsi.o) \
	$(X_WGLLIB)(x_wglias.o) \
	$(X_WGLLIB)(x_wglinicmap.o) \
	$(X_WGLLIB)(x_wglinids.o) \
	$(X_WGLLIB)(x_wglinipat.o) \
	$(X_WGLLIB)(x_wglinit.o) \
	$(X_WGLLIB)(x_wgliniwin.o) \
	$(X_WGLLIB)(x_wgliniwpx.o) \
	$(X_WGLLIB)(x_wglinstcmap.o) \
	$(X_WGLLIB)(x_wglinvpat.o) \
	$(X_WGLLIB)(x_wglkas.o) \
	$(X_WGLLIB)(x_wgllpt.o) \
	$(X_WGLLIB)(x_wgllwi.o) \
	$(X_WGLLIB)(x_wglmapc.o) \
	$(X_WGLLIB)(x_wglmapcs.o) \
	$(X_WGLLIB)(x_wglmco.o) \
	$(X_WGLLIB)(x_wglmcos.o) \
	$(X_WGLLIB)(x_wglmcw.o) \
	$(X_WGLLIB)(x_wglmesagr.o) \
	$(X_WGLLIB)(x_wglmvi.o) \
	$(X_WGLLIB)(x_wglncl.o) \
	$(X_WGLLIB)(x_wglnewwin.o) \
	$(X_WGLLIB)(x_wglopmw.o) \
	$(X_WGLLIB)(x_wglopw.o) \
	$(X_WGLLIB)(x_wglpfi.o) \
	$(X_WGLLIB)(x_wglpli.o) \
	$(X_WGLLIB)(x_wglppo.o) \
	$(X_WGLLIB)(x_wglpsi.o) \
	$(X_WGLLIB)(x_wglpsz.o) \
	$(X_WGLLIB)(x_wglpti.o) \
	$(X_WGLLIB)(x_wglptis.o) \
	$(X_WGLLIB)(x_wglptxs.o) \
	$(X_WGLLIB)(x_wglrfi.o) \
	$(X_WGLLIB)(x_wglrli.o) \
	$(X_WGLLIB)(x_wglroc.o) \
	$(X_WGLLIB)(x_wglrstdpar.o) \
	$(X_WGLLIB)(x_wglrwc.o) \
	$(X_WGLLIB)(x_wglsavpfc.o) \
	$(X_WGLLIB)(x_wglsbf.o) \
	$(X_WGLLIB)(x_wglsetw.o) \
	$(X_WGLLIB)(x_wglsetwcmap.o) \
	$(X_WGLLIB)(x_wglsld.o) \
	$(X_WGLLIB)(x_wglsmk.o) \
	$(X_WGLLIB)(x_wglspt.o) \
	$(X_WGLLIB)(x_wglstu.o) \
	$(X_WGLLIB)(x_wglswb.o) \
	$(X_WGLLIB)(x_wglwsi.o) \
	$(X_WGLLIB)(x_wglwtwrsz.o) \
	$(X_WGLLIB)(x_wglxai.o) \
	$(X_WGLLIB)(x_xmmenu.o) \
	$(X_WGLLIB)(x_invertChar.o) \
	$(X_WGLLIB)(x_wwwlpt.o) \
	$(X_WGLLIB)(x_wglmesagr.o)




