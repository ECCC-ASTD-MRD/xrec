include Makefile_$(ARCH)$(ABI)

.SUFFIXES : 

.SUFFIXES : .ftn .f .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

X_WGLLIB = $(REC)/lib/$(ARCH)$(ABI)/libx_wgl.a

FFLAGS = 

CFLAGS =  -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I$(REC)/include -DX_WGL

OPTIMIZ = -debug
OPTIMIZ = -O 2

CPPFLAGS = -I$(ARMNLIB)/include =-I$(REC)/include -DX_WGL

.PRECIOUS:

default: x_wgllib

.ftn.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $*.f

.ftn.f:
	rm -f $*.f
	r.ftntof -P $(DEFINE) $< > $*.f
	chmod 444 $*.f

.c.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.f.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.a:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.o

.f.a:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.o

FTNDECKS= \
         unsat.ftn       wwwlpt.ftn

FDECKS= \
         unsat.f       wwwlpt.f

CDECKS= \
         x_bitfinder.c       x_couleur-util.c       x_initdev.c       x_inversion.c \
         x_logo_sea.c       x_pwrit_x.c       x_souris2.c       x_wglafi.c \
         x_wglafx.c       x_wglali.c       x_wglalx.c       x_wglbbf.c \
         x_wglbrb.c       x_wglbti.c       x_wglbtn.c       x_wglbtp.c \
         x_wglbtpi.c       x_wglbtpx.c       x_wglbtx.c       x_wglcfi.c \
         x_wglcfx.c       x_wglchngfs.c       x_wglcli.c       x_wglclr.c \
         x_wglclw.c       x_wglclx.c       x_wglcmi.c       x_wglcmx.c \
         x_wglcol.c       x_wglcolf.c       x_wglcrb.c       x_wgldbf.c \
         x_wgldbg.c       x_wgldcm.c       x_wgldeflst.c       x_wgldefncw.c \
         x_wgldld.c       x_wgldpt.c       x_wgldri.c       x_wgldrx.c \
         x_wglerb.c       x_wglfbf.c       x_wglfsc.c       x_wglfsh.c \
         x_wglfshlb.c       x_wglgacw.c       x_wglgaf.c       x_wglgbf.c \
         x_wglgci.c       x_wglgco.c       x_wglgdbg.c       x_wglgetcmap.c \
         x_wglgld.c       x_wglglpti.c       x_wglglw.c       x_wglgmk.c \
         x_wglgmod.c       x_wglgpl.c       x_wglgpt.c       x_wglgsi.c \
         x_wglgsp.c       x_wglgsx.c       x_wglgvi.c       x_wglgvp.c \
         x_wglgvx.c       x_wglgwz.c       x_wglgzi.c       x_wglgzp.c \
         x_wglgzx.c       x_wglias.c       x_wgliax.c       x_wglinicmap.c \
         x_wglinids.c       x_wglinipat.c       x_wglinit.c       x_wgliniwin.c \
         x_wgliniwpx.c       x_wglinstcmap.c       x_wglinvpat.c       x_wglkas.c \
         x_wgllpt.c       x_wgllwi.c       x_wglmapc.c       x_wglmapcs.c \
         x_wglmco.c       x_wglmcos.c       x_wglmcw.c       x_wglmesagr.c \
         x_wglmvi.c       x_wglmvx.c       x_wglncl.c       x_wglnewwin.c \
         x_wglopmw.c       x_wglopw.c       x_wglpfi.c       x_wglpfx.c \
         x_wglpli.c       x_wglplx.c       x_wglppo.c       x_wglpsz.c \
         x_wglpti.c       x_wglptis.c       x_wglptx.c       x_wglrfi.c \
         x_wglrfx.c       x_wglrli.c       x_wglrlx.c       x_wglroc.c \
         x_wglrstdpar.c       x_wglrwc.c       x_wglsavpfc.c       x_wglsbf.c \
         x_wglsetw.c       x_wglsetwcmap.c       x_wglsld.c       x_wglsmk.c \
         x_wglspt.c       x_wglssp.c       x_wglstu.c       x_wglswb.c \
         x_wglwtwrsz.c       x_wglxai.c       x_xmmenu.c

FDECKS= \
         x_unsat.f       x_wwwlpt.f

OBJECTS= \
         x_bitfinder.o       x_couleur-util.o       x_initdev.o       x_inversion.o \
         x_logo_sea.o       x_pwrit_x.o       x_souris2.o       x_unsat.o \
         x_wglafi.o       x_wglafx.o       x_wglali.o       x_wglalx.o \
         x_wglbbf.o       x_wglbrb.o       x_wglbti.o       x_wglbtn.o \
         x_wglbtp.o       x_wglbtpi.o       x_wglbtpx.o       x_wglbtx.o \
         x_wglcfi.o       x_wglcfx.o       x_wglchngfs.o       x_wglcli.o \
         x_wglclr.o       x_wglclw.o       x_wglclx.o       x_wglcmi.o \
         x_wglcmx.o       x_wglcol.o       x_wglcolf.o       x_wglcrb.o \
         x_wgldbf.o       x_wgldbg.o       x_wgldcm.o       x_wgldeflst.o \
         x_wgldefncw.o       x_wgldld.o       x_wgldpt.o       x_wgldri.o \
         x_wgldrx.o       x_wglerb.o       x_wglfbf.o       x_wglfsc.o \
         x_wglfsh.o       x_wglfshlb.o       x_wglgacw.o       x_wglgaf.o \
         x_wglgbf.o       x_wglgci.o       x_wglgco.o       x_wglgdbg.o \
         x_wglgetcmap.o       x_wglgld.o       x_wglglpti.o       x_wglglw.o \
         x_wglgmk.o       x_wglgmod.o       x_wglgpl.o       x_wglgpt.o \
         x_wglgsi.o       x_wglgsp.o       x_wglgsx.o       x_wglgvi.o \
         x_wglgvp.o       x_wglgvx.o       x_wglgwz.o       x_wglgzi.o \
         x_wglgzp.o       x_wglgzx.o       x_wglias.o       x_wgliax.o \
         x_wglinicmap.o       x_wglinids.o       x_wglinipat.o       x_wglinit.o \
         x_wgliniwin.o       x_wgliniwpx.o       x_wglinstcmap.o       x_wglinvpat.o \
         x_wglkas.o       x_wgllpt.o       x_wgllwi.o       x_wglmapc.o \
         x_wglmapcs.o       x_wglmco.o       x_wglmcos.o       x_wglmcw.o \
         x_wglmesagr.o       x_wglmvi.o       x_wglmvx.o       x_wglncl.o \
         x_wglnewwin.o       x_wglopmw.o       x_wglopw.o       x_wglpfi.o \
         x_wglpfx.o       x_wglpli.o       x_wglplx.o       x_wglppo.o \
         x_wglpsz.o       x_wglpti.o       x_wglptis.o       x_wglptx.o \
         x_wglrfi.o       x_wglrfx.o       x_wglrli.o       x_wglrlx.o \
         x_wglroc.o       x_wglrstdpar.o       x_wglrwc.o       x_wglsavpfc.o \
         x_wglsbf.o       x_wglsetw.o       x_wglsetwcmap.o       x_wglsld.o \
         x_wglsmk.o       x_wglspt.o       x_wglssp.o       x_wglstu.o \
         x_wglswb.o       x_wglwtwrsz.o       x_wglxai.o       x_wwwlpt.o \
         x_xmmenu.o

unsat.f: x_unsat.ftn 
wwwlpt.f: x_wwwlpt.ftn 

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

x_wgllib: $(CDECKS) $(FDECKS) \
	$(X_WGLLIB)(x_bitfinder.o) \
	$(X_WGLLIB)(x_couleur-util.o) \
	$(X_WGLLIB)(x_initdev.o) \
	$(X_WGLLIB)(x_inversion.o) \
	$(X_WGLLIB)(x_logo_sea.o) \
	$(X_WGLLIB)(x_pwrit_x.o) \
	$(X_WGLLIB)(x_souris2.o) \
	$(X_WGLLIB)(x_unsat.o) \
	$(X_WGLLIB)(x_wglafi.o) \
	$(X_WGLLIB)(x_wglafx.o) \
	$(X_WGLLIB)(x_wglali.o) \
	$(X_WGLLIB)(x_wglalx.o) \
	$(X_WGLLIB)(x_wglbbf.o) \
	$(X_WGLLIB)(x_wglbrb.o) \
	$(X_WGLLIB)(x_wglbti.o) \
	$(X_WGLLIB)(x_wglbtn.o) \
	$(X_WGLLIB)(x_wglbtp.o) \
	$(X_WGLLIB)(x_wglbtpi.o) \
	$(X_WGLLIB)(x_wglbtpx.o) \
	$(X_WGLLIB)(x_wglbtx.o) \
	$(X_WGLLIB)(x_wglcfi.o) \
	$(X_WGLLIB)(x_wglcfx.o) \
	$(X_WGLLIB)(x_wglchngfs.o) \
	$(X_WGLLIB)(x_wglcli.o) \
	$(X_WGLLIB)(x_wglclr.o) \
	$(X_WGLLIB)(x_wglclw.o) \
	$(X_WGLLIB)(x_wglclx.o) \
	$(X_WGLLIB)(x_wglcmi.o) \
	$(X_WGLLIB)(x_wglcmx.o) \
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
	$(X_WGLLIB)(x_wgldrx.o) \
	$(X_WGLLIB)(x_wglerb.o) \
	$(X_WGLLIB)(x_wglfbf.o) \
	$(X_WGLLIB)(x_wglfsc.o) \
	$(X_WGLLIB)(x_wglfsh.o) \
	$(X_WGLLIB)(x_wglfshlb.o) \
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
	$(X_WGLLIB)(x_wglgsx.o) \
	$(X_WGLLIB)(x_wglgvi.o) \
	$(X_WGLLIB)(x_wglgvp.o) \
	$(X_WGLLIB)(x_wglgvx.o) \
	$(X_WGLLIB)(x_wglgwz.o) \
	$(X_WGLLIB)(x_wglgzi.o) \
	$(X_WGLLIB)(x_wglgzp.o) \
	$(X_WGLLIB)(x_wglgzx.o) \
	$(X_WGLLIB)(x_wglias.o) \
	$(X_WGLLIB)(x_wgliax.o) \
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
	$(X_WGLLIB)(x_wglmvx.o) \
	$(X_WGLLIB)(x_wglncl.o) \
	$(X_WGLLIB)(x_wglnewwin.o) \
	$(X_WGLLIB)(x_wglopmw.o) \
	$(X_WGLLIB)(x_wglopw.o) \
	$(X_WGLLIB)(x_wglpfi.o) \
	$(X_WGLLIB)(x_wglpfx.o) \
	$(X_WGLLIB)(x_wglpli.o) \
	$(X_WGLLIB)(x_wglplx.o) \
	$(X_WGLLIB)(x_wglppo.o) \
	$(X_WGLLIB)(x_wglpsz.o) \
	$(X_WGLLIB)(x_wglpti.o) \
	$(X_WGLLIB)(x_wglptis.o) \
	$(X_WGLLIB)(x_wglptx.o) \
	$(X_WGLLIB)(x_wglptxs.o) \
	$(X_WGLLIB)(x_wglrfi.o) \
	$(X_WGLLIB)(x_wglrfx.o) \
	$(X_WGLLIB)(x_wglrli.o) \
	$(X_WGLLIB)(x_wglrlx.o) \
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
	$(X_WGLLIB)(x_wglssp.o) \
	$(X_WGLLIB)(x_wglstu.o) \
	$(X_WGLLIB)(x_wglswb.o) \
	$(X_WGLLIB)(x_wglwtwrsz.o) \
	$(X_WGLLIB)(x_wglxai.o) \
	$(X_WGLLIB)(x_wwwlpt.o) \
	$(X_WGLLIB)(x_xmmenu.o)


