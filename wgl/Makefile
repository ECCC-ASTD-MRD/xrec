include Makefile_$(ARCH)$(ABI)

.SUFFIXES : 

.SUFFIXES : .ftn .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

WGLLIB = $(REC)/lib/$(ARCH)$(ABI)/libwgl.a

FFLAGS = 

CFLAGS =  -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I$(REC)/include -DX_WGL

OPTIMIZ = -debug
OPTIMIZ = -O 2

CPPFLAGS = -I$(ARMNLIB)/include =-I$(REC)/include -DX_WGL

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

FDECKS= \
         unsat.f       wwwlpt.f

CDECKS= \
         bitfinder.c       couleur-util.c       initdev.c       inversion.c \
         logo_sea.c       pwrit_x.c       souris2.c       wglafi.c \
         wglafx.c       wglali.c       wglalx.c       wglbbf.c \
         wglbrb.c       wglbti.c       wglbtn.c       wglbtp.c \
         wglbtpi.c       wglbtpx.c       wglbtx.c       wglcfi.c \
         wglcfx.c       wglchngfs.c       wglcli.c       wglclr.c \
         wglclw.c       wglclx.c       wglcmi.c       wglcmx.c \
         wglcol.c       wglcolf.c       wglcrb.c       wgldbf.c \
         wgldbg.c       wgldcm.c       wgldeflst.c       wgldefncw.c \
         wgldld.c       wgldpt.c       wgldri.c       wgldrx.c \
         wglerb.c       wglfbf.c       wglfsc.c       wglfsh.c \
         wglfshlb.c       wglgacw.c       wglgaf.c       wglgbf.c \
         wglgci.c       wglgco.c       wglgdbg.c       wglgetcmap.c \
         wglgld.c       wglglpti.c       wglglw.c       wglgmk.c \
         wglgmod.c       wglgpl.c       wglgpt.c       wglgsi.c \
         wglgsp.c       wglgsx.c       wglgvi.c       wglgvp.c \
         wglgvx.c       wglgwz.c       wglgzi.c       wglgzp.c \
         wglgzx.c       wglias.c       wgliax.c       wglinicmap.c \
         wglinids.c       wglinipat.c       wglinit.c       wgliniwin.c \
         wgliniwpx.c       wglinstcmap.c       wglinvpat.c       wglkas.c \
         wgllpt.c       wgllwi.c       wglmapc.c       wglmapcs.c \
         wglmco.c       wglmcos.c       wglmcw.c       wglmesagr.c \
         wglmvi.c       wglmvx.c       wglncl.c       wglnewwin.c \
         wglopmw.c       wglopw.c       wglpfi.c       wglpfx.c \
         wglpli.c       wglplx.c       wglppo.c       wglpsz.c \
         wglpti.c       wglptis.c       wglptx.c       wglrfi.c \
         wglrfx.c       wglrli.c       wglrlx.c       wglroc.c \
         wglrstdpar.c       wglrwc.c       wglsavpfc.c       wglsbf.c \
         wglsetw.c       wglsetwcmap.c       wglsld.c       wglsmk.c \
         wglspt.c       wglssp.c       wglstu.c       wglswb.c \
         wglwtwrsz.c       wglxai.c       xmmenu.c

FDECKS= \
         unsat.f       wwwlpt.f

OBJECTS= \
         bitfinder.o       couleur-util.o       initdev.o       inversion.o \
         logo_sea.o       pwrit_x.o       souris2.o       unsat.o \
         wglafi.o       wglafx.o       wglali.o       wglalx.o \
         wglbbf.o       wglbrb.o       wglbti.o       wglbtn.o \
         wglbtp.o       wglbtpi.o       wglbtpx.o       wglbtx.o \
         wglcfi.o       wglcfx.o       wglchngfs.o       wglcli.o \
         wglclr.o       wglclw.o       wglclx.o       wglcmi.o \
         wglcmx.o       wglcol.o       wglcolf.o       wglcrb.o \
         wgldbf.o       wgldbg.o       wgldcm.o       wgldeflst.o \
         wgldefncw.o       wgldld.o       wgldpt.o       wgldri.o \
         wgldrx.o       wglerb.o       wglfbf.o       wglfsc.o \
         wglfsh.o       wglfshlb.o       wglgacw.o       wglgaf.o \
         wglgbf.o       wglgci.o       wglgco.o       wglgdbg.o \
         wglgetcmap.o       wglgld.o       wglglpti.o       wglglw.o \
         wglgmk.o       wglgmod.o       wglgpl.o       wglgpt.o \
         wglgsi.o       wglgsp.o       wglgsx.o       wglgvi.o \
         wglgvp.o       wglgvx.o       wglgwz.o       wglgzi.o \
         wglgzp.o       wglgzx.o       wglias.o       wgliax.o \
         wglinicmap.o       wglinids.o       wglinipat.o       wglinit.o \
         wgliniwin.o       wgliniwpx.o       wglinstcmap.o       wglinvpat.o \
         wglkas.o       wgllpt.o       wgllwi.o       wglmapc.o \
         wglmapcs.o       wglmco.o       wglmcos.o       wglmcw.o \
         wglmesagr.o       wglmvi.o       wglmvx.o       wglncl.o \
         wglnewwin.o       wglopmw.o       wglopw.o       wglpfi.o \
         wglpfx.o       wglpli.o       wglplx.o       wglppo.o \
         wglpsz.o       wglpti.o       wglptis.o       wglptx.o \
         wglrfi.o       wglrfx.o       wglrli.o       wglrlx.o \
         wglroc.o       wglrstdpar.o       wglrwc.o       wglsavpfc.o \
         wglsbf.o       wglsetw.o       wglsetwcmap.o       wglsld.o \
         wglsmk.o       wglspt.o       wglssp.o       wglstu.o \
         wglswb.o       wglwtwrsz.o       wglxai.o       wwwlpt.o \
         xmmenu.o

unsat.f: unsat.ftn 
wwwlpt.f: wwwlpt.ftn 

obj: $(CDECKS) $(FDECKS) $(OBJECTS) 
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

wgllib: $(WGLLIB)(bitfinder.o) \
	$(WGLLIB)(couleur-util.o) \
	$(WGLLIB)(initdev.o) \
	$(WGLLIB)(inversion.o) \
	$(WGLLIB)(logo_sea.o) \
	$(WGLLIB)(pwrit_x.o) \
	$(WGLLIB)(souris2.o) \
	$(WGLLIB)(unsat.o) \
	$(WGLLIB)(wglafi.o) \
	$(WGLLIB)(wglafx.o) \
	$(WGLLIB)(wglali.o) \
	$(WGLLIB)(wglalx.o) \
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
	$(WGLLIB)(wglerb.o) \
	$(WGLLIB)(wglfbf.o) \
	$(WGLLIB)(wglfsc.o) \
	$(WGLLIB)(wglfsh.o) \
	$(WGLLIB)(wglfshlb.o) \
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
	$(WGLLIB)(wglias.o) \
	$(WGLLIB)(wgliax.o) \
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
	$(WGLLIB)(wglsetw.o) \
	$(WGLLIB)(wglsetwcmap.o) \
	$(WGLLIB)(wglsld.o) \
	$(WGLLIB)(wglsmk.o) \
	$(WGLLIB)(wglspt.o) \
	$(WGLLIB)(wglssp.o) \
	$(WGLLIB)(wglstu.o) \
	$(WGLLIB)(wglswb.o) \
	$(WGLLIB)(wglwtwrsz.o) \
	$(WGLLIB)(wglxai.o) \
	$(WGLLIB)(wwwlpt.o) \
	$(WGLLIB)(xmmenu.o)


