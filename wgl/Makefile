include /usr/local/env/armnlib/include/$(ARCH)$(ABI)/Makefile_addons

.SUFFIXES : 

.SUFFIXES : .ftn .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

WGLLIB = $(HOME)/userlibs/$(ARCH)$(ABI)/libwgl.a

FFLAGS = 

CFLAGS =  -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I../include -DX_WGL

OPTIMIZ = -debug -O 0
OPTIMIZ = -O 3

CPPFLAGS = -I$(ARMNLIB)/include =-I../include -DX_WGL

.PRECIOUS:

default: wgllib

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

FTNDECKS= \
         unsat.ftn       wwwlpt.ftn

CDECKS= \
	bitfinder.c   couleur-util.c  gdwgl.c	   inversion.c\
	langue.c      logo.c	      logo_sea.c   souris2.c   wglafi.c\
	wglafx.c      wglali.c	      wglalx.c	   wglasi.c	 wglasx.c\
	wglbbf.c      wglbrb.c	      wglbti.c	   wglbtn.c	 wglbtp.c\
	wglbtpi.c     wglbtpx.c       wglbtx.c	   wglcfi.c	 wglcfx.c\
	wglchngfs.c   wglcli.c	      wglclr.c	   wglclw.c	 wglclx.c\
	wglcmi.c      wglcmx.c	      wglcol.c	   wglcolf.c	 wglcrb.c\
	wgldbf.c      wgldbg.c	      wgldcm.c	   wgldeflst.c	 wgldefncw.c\
	wgldld.c      wgldpt.c	      wgldri.c	   wgldrx.c	 wgldsi.c\
	wgldsx.c      wglecr.c 	      wglerb.c	      wglfbf.c	   wglfsc.c	 wglfsh.c\
	wglfshlb.c    wglfsz.c	      wglgacw.c    wglgaf.c	 wglgbf.c\
	wglgci.c      wglgco.c	      wglgdbg.c    wglgetcmap.c  wglgld.c\
	wglglpti.c    wglglw.c	      wglgmk.c	   wglgmod.c	 wglgpl.c\
	wglgpt.c      wglgsi.c	      wglgsp.c	   wglgsx.c	 wglgvi.c\
	wglgvp.c      wglgvx.c	      wglgwz.c	   wglgzi.c	 wglgzp.c\
	wglgzx.c      wglhsi.c	      wglhsx.c	   wglias.c	 wgliax.c wgligd.c \
	wglinicmap.c  wglinids.c      wglinipat.c  wglinit.c	 wgliniwin.c\
	wgliniwpx.c   wglinstcmap.c   wglinvpat.c  wglkas.c	 wgllpt.c\
	wgllwi.c      wglmapc.c       wglmapcs.c   wglmco.c	 wglmcos.c\
	wglmcw.c      wglmesagr.c     wglmvi.c	   wglmvx.c	 wglncl.c\
	wglnewwin.c   wglopmw.c       wglopw.c	   wglpfi.c	 wglpfx.c\
	wglpli.c      wglplx.c	      wglppo.c	   wglpsi.c	 wglpsx.c\
	wglpsz.c      wglpti.c	      wglptis.c    wglptx.c	 wglptxs.c\
	wglrfi.c      wglrfx.c	      wglrli.c	   wglrlx.c	 wglroc.c\
	wglrstdpar.c  wglrwc.c	      wglsavpfc.c  wglsbf.c	 wglscon.c\
	wglsetw.c     wglsetwcmap.c   wglsld.c	   wglsmk.c	 wglspt.c\
	wglssp.c      wglstu.c	      wglswb.c	   wglwsi.c	 wglwsx.c\
	wglwtwrsz.c   wglxai.c

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

wgllib:	$(WGLLIB)(bitfinder.o) \
	$(WGLLIB)(couleur-util.o) \
	$(WGLLIB)(langue.o) \
	$(WGLLIB)(gdwgl.o) \
	$(WGLLIB)(inversion.o) \
	$(WGLLIB)(logo.o) \
	$(WGLLIB)(logo_sea.o) \
	$(WGLLIB)(souris2.o) \
	$(WGLLIB)(unsat.o) \
	$(WGLLIB)(wglafi.o) \
	$(WGLLIB)(wglafx.o) \
	$(WGLLIB)(wglali.o) \
	$(WGLLIB)(wglalx.o) \
	$(WGLLIB)(wglasi.o) \
	$(WGLLIB)(wglasx.o) \
	$(WGLLIB)(wglbbf.o) \
	$(WGLLIB)(wglbrb.o) \
	$(WGLLIB)(wglbti.o) \
	$(WGLLIB)(wglbtn.o) \
	$(WGLLIB)(wglbtp.o) \
	$(WGLLIB)(wglbtpi.o) \
	$(WGLLIB)(wglbtpx.o) \
	$(WGLLIB)(wglbtx.o) \
	$(WGLLIB)(wglcfi.o) \
	$(WGLLIB)(wglcfx.o) \
	$(WGLLIB)(wglchngfs.o) \
	$(WGLLIB)(wglcli.o) \
	$(WGLLIB)(wglclr.o) \
	$(WGLLIB)(wglclw.o) \
	$(WGLLIB)(wglclx.o) \
	$(WGLLIB)(wglcmi.o) \
	$(WGLLIB)(wglcmx.o) \
	$(WGLLIB)(wglcol.o) \
	$(WGLLIB)(wglcolf.o) \
	$(WGLLIB)(wglcrb.o) \
	$(WGLLIB)(wgldbf.o) \
	$(WGLLIB)(wgldbg.o) \
	$(WGLLIB)(wgldcm.o) \
	$(WGLLIB)(wgldeflst.o) \
	$(WGLLIB)(wgldefncw.o) \
	$(WGLLIB)(wgldld.o) \
	$(WGLLIB)(wgldpt.o) \
	$(WGLLIB)(wgldri.o) \
	$(WGLLIB)(wgldrx.o) \
	$(WGLLIB)(wgldsi.o) \
	$(WGLLIB)(wgldsx.o) \
	$(WGLLIB)(wglecr.o) \
	$(WGLLIB)(wglerb.o) \
	$(WGLLIB)(wglfbf.o) \
	$(WGLLIB)(wglfsc.o) \
	$(WGLLIB)(wglfsh.o) \
	$(WGLLIB)(wglfshlb.o) \
	$(WGLLIB)(wglfsz.o) \
	$(WGLLIB)(wglgacw.o) \
	$(WGLLIB)(wglgaf.o) \
	$(WGLLIB)(wglgbf.o) \
	$(WGLLIB)(wglgci.o) \
	$(WGLLIB)(wglgco.o) \
	$(WGLLIB)(wglgdbg.o) \
	$(WGLLIB)(wglgetcmap.o) \
	$(WGLLIB)(wglgld.o) \
	$(WGLLIB)(wglglpti.o) \
	$(WGLLIB)(wglglw.o) \
	$(WGLLIB)(wglgmk.o) \
	$(WGLLIB)(wglgmod.o) \
	$(WGLLIB)(wglgpl.o) \
	$(WGLLIB)(wglgpt.o) \
	$(WGLLIB)(wglgsi.o) \
	$(WGLLIB)(wglgsp.o) \
	$(WGLLIB)(wglgsx.o) \
	$(WGLLIB)(wglgvi.o) \
	$(WGLLIB)(wglgvp.o) \
	$(WGLLIB)(wglgvx.o) \
	$(WGLLIB)(wglgwz.o) \
	$(WGLLIB)(wglgzi.o) \
	$(WGLLIB)(wglgzp.o) \
	$(WGLLIB)(wglgzx.o) \
	$(WGLLIB)(wglhsi.o) \
	$(WGLLIB)(wglhsx.o) \
	$(WGLLIB)(wglias.o) \
	$(WGLLIB)(wgliax.o) \
	$(WGLLIB)(wgligd.o) \
	$(WGLLIB)(wglinicmap.o) \
	$(WGLLIB)(wglinids.o) \
	$(WGLLIB)(wglinipat.o) \
	$(WGLLIB)(wglinit.o) \
	$(WGLLIB)(wgliniwin.o) \
	$(WGLLIB)(wgliniwpx.o) \
	$(WGLLIB)(wglinstcmap.o) \
	$(WGLLIB)(wglinvpat.o) \
	$(WGLLIB)(wglkas.o) \
	$(WGLLIB)(wgllpt.o) \
	$(WGLLIB)(wgllwi.o) \
	$(WGLLIB)(wglmapc.o) \
	$(WGLLIB)(wglmapcs.o) \
	$(WGLLIB)(wglmco.o) \
	$(WGLLIB)(wglmcos.o) \
	$(WGLLIB)(wglmcw.o) \
	$(WGLLIB)(wglmesagr.o) \
	$(WGLLIB)(wglmvi.o) \
	$(WGLLIB)(wglmvx.o) \
	$(WGLLIB)(wglncl.o) \
	$(WGLLIB)(wglnewwin.o) \
	$(WGLLIB)(wglopmw.o) \
	$(WGLLIB)(wglopw.o) \
	$(WGLLIB)(wglpfi.o) \
	$(WGLLIB)(wglpfx.o) \
	$(WGLLIB)(wglpli.o) \
	$(WGLLIB)(wglplx.o) \
	$(WGLLIB)(wglppo.o) \
	$(WGLLIB)(wglpsi.o) \
	$(WGLLIB)(wglpsx.o) \
	$(WGLLIB)(wglpsz.o) \
	$(WGLLIB)(wglpti.o) \
	$(WGLLIB)(wglptis.o) \
	$(WGLLIB)(wglptx.o) \
	$(WGLLIB)(wglptxs.o) \
	$(WGLLIB)(wglrfi.o) \
	$(WGLLIB)(wglrfx.o) \
	$(WGLLIB)(wglrli.o) \
	$(WGLLIB)(wglrlx.o) \
	$(WGLLIB)(wglroc.o) \
	$(WGLLIB)(wglrstdpar.o) \
	$(WGLLIB)(wglrwc.o) \
	$(WGLLIB)(wglsavpfc.o) \
	$(WGLLIB)(wglsbf.o) \
	$(WGLLIB)(wglscon.o) \
	$(WGLLIB)(wglsetw.o) \
	$(WGLLIB)(wglsetwcmap.o) \
	$(WGLLIB)(wglsld.o) \
	$(WGLLIB)(wglsmk.o) \
	$(WGLLIB)(wglspt.o) \
	$(WGLLIB)(wglssp.o) \
	$(WGLLIB)(wglstu.o) \
	$(WGLLIB)(wglswb.o) \
	$(WGLLIB)(wglwsi.o) \
	$(WGLLIB)(wglwsx.o) \
	$(WGLLIB)(wglwtwrsz.o) \
	$(WGLLIB)(wglxai.o) \
	$(WGLLIB)(wwwlpt.o)
