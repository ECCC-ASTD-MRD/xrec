#include Makefile_$(ARCH)$(ABI)

.SUFFIXES : 

.SUFFIXES : .ftn .f .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

RECLIB = $(REC)/lib/$(ARCH)$(ABI)/librec.a

FFLAGS = -I$(ARMNLIB)/include -I$(REC)/include -DX_WGL

CFLAGS = -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I$(REC)/include -I$(HOME)/src/gd2/ -DX_WGL

#@OPTIMIZ = -O 3
OPTIMIZ = -debug -O 0

CPPFLAGS = -I$(ARMNLIB)/include =-I$(REC)/include -DX_WGL

.PRECIOUS: $(RECLIB)

default: reclib

.ftn.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

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
         convip.ftn       convpr.ftn       defvar.ftn       drlstl.ftn \
         grid.ftn              new-xfsl.ftn       readrec.ftn \
         rec.ftn       recgrille.ftn       rlx.ftn       setcoupe.ftn \
         setitem.ftn       statfld4.ftn       unsat.ftn       wglrfton.ftn

CDECKS= \
         PointerVent.c       aff_coupe.c       anim4.c       aux.c \
         bidim.c       cmap.c       contour.c       couleur.c \
         coupe.c       dict.c       diffmgr.c       enhance.c \
         fenlatlon.c       fermeur.c       fldmgr.c       fleche.c \
         geometrie.c       grid_status.c       grille.c \
         hilo.c       i2rgdint.c       igd.c       inversion.c \
         lclevent.c       legende.c       logo_rpn.c \
         manipule.c       mapinfo.c       message.c       no_rmnlib.c \
         nocore.c       opRefusee.c       p_anim.c       p_attr.c \
         p_avert.c       p_contour.c       p_coupe.c       p_diapo.c \
         p_edition.c       p_geo.c       p_minmax.c       p_palette.c \
         p_vent.c       pack.c       parent.c \
         profil.c       ptval.c       rle.c       sfcpres.c \
         strutil.c       util.c       valptgr.c       vent.c \
         wglanul.c       wglcalcols.c       wglcont.c       wglfton.c \
         wgllab.c       wglpfton.c       wglpfton32.c       widgets-util.c \
         xm-controle.c

OBJECTS= \
         convip.o       convpr.o       defvar.o       drlstl.o \
         grid.o              new-xfsl.o       readrec.o \
         rec.o       recgrille.o       rlx.o       setcoupe.o \
         setitem.o       statfld4.o       unsat.o       wglrfton.o \
         PointerVent.o       aff_coupe.o       anim4.o       aux.o \
         bidim.o       cmap.o       contour.o       convip.o \
         convpr.o       couleur.o       coupe.o       defvar.o \
         dict.o       diffmgr.o       drlstl.o       enhance.o \
         fenlatlon.o       fermeur.o       fldmgr.o       fleche.o \
         geometrie.o       grid.o       grid_status.o \
         grille.o       hilo.o       i2rgdint.o       igd.o \
         inversion.o              lclevent.o       legende.o \
                logo_rpn.o       manipule.o       mapinfo.o \
         message.o       new-xfsl.o       no_rmnlib.o       nocore.o \
         opRefusee.o       p_anim.o       p_attr.o       p_avert.o \
         p_contour.o       p_coupe.o       p_diapo.o       p_edition.o \
         p_geo.o       p_minmax.o       p_palette.o        \
         p_vent.o       pack.o       parent.o       profil.o \
         ptval.o       readrec.o       rec.o       recgrille.o \
         rle.o       rlx.o       setcoupe.o       setitem.o \
         sfcpres.o       statfld4.o       strutil.o       unsat.o \
         util.o       valptgr.o       vent.o       wglanul.o \
         wglcalcols.o       wglcont.o       wglfton.o       wgllab.o \
         wglpfton.o       wglpfton32.o       wglrfton.o       widgets-util.o \
         xm-controle.o

COMDECKS= \
         impnone.cdk       qqqxtrp.cdk

reclib: $(CDECKS) $(FTNDECKS) $(OBJECTS)\
	$(RECLIB)(convip.o) \
	$(RECLIB)(convpr.o) \
	$(RECLIB)(defvar.o) \
	$(RECLIB)(drlstl.o) \
	$(RECLIB)(readrec.o) \
	$(RECLIB)(grid.o) \
	$(RECLIB)(new-xfsl.o) \
	$(RECLIB)(rec.o) \
	$(RECLIB)(grid_status.o) \
	$(RECLIB)(recgrille.o) \
	$(RECLIB)(rlx.o) \
	$(RECLIB)(setcoupe.o) \
	$(RECLIB)(setitem.o) \
	$(RECLIB)(statfld4.o) \
	$(RECLIB)(unsat.o) \
	$(RECLIB)(wglrfton.o) \
	$(RECLIB)(PointerVent.o) \
	$(RECLIB)(aff_coupe.o) \
	$(RECLIB)(anim4.o) \
	$(RECLIB)(aux.o) \
	$(RECLIB)(bidim.o) \
	$(RECLIB)(cmap.o) \
	$(RECLIB)(contour.o) \
	$(RECLIB)(couleur.o) \
	$(RECLIB)(coupe.o) \
	$(RECLIB)(dict.o) \
	$(RECLIB)(diffmgr.o) \
	$(RECLIB)(enhance.o) \
	$(RECLIB)(fenlatlon.o) \
	$(RECLIB)(fermeur.o) \
	$(RECLIB)(fldmgr.o) \
	$(RECLIB)(fleche.o) \
	$(RECLIB)(geometrie.o) \
	$(RECLIB)(grid.o) \
	$(RECLIB)(grille.o) \
	$(RECLIB)(hilo.o) \
	$(RECLIB)(i2rgdint.o) \
	$(RECLIB)(igd.o) \
	$(RECLIB)(inversion.o) \
	$(RECLIB)(lclevent.o) \
	$(RECLIB)(legende.o) \
	$(RECLIB)(logo_rpn.o) \
	$(RECLIB)(manipule.o) \
	$(RECLIB)(mapinfo.o) \
	$(RECLIB)(message.o) \
	$(RECLIB)(no_rmnlib.o) \
	$(RECLIB)(nocore.o) \
	$(RECLIB)(opRefusee.o) \
	$(RECLIB)(p_anim.o) \
	$(RECLIB)(p_attr.o) \
	$(RECLIB)(p_avert.o) \
	$(RECLIB)(p_contour.o) \
	$(RECLIB)(p_coupe.o) \
	$(RECLIB)(p_diapo.o) \
	$(RECLIB)(p_edition.o) \
	$(RECLIB)(p_geo.o) \
	$(RECLIB)(p_minmax.o) \
	$(RECLIB)(p_palette.o) \
	$(RECLIB)(p_vent.o) \
	$(RECLIB)(pack.o) \
	$(RECLIB)(parent.o) \
	$(RECLIB)(profil.o) \
	$(RECLIB)(ptval.o) \
	$(RECLIB)(rle.o) \
	$(RECLIB)(sfcpres.o) \
	$(RECLIB)(strutil.o) \
	$(RECLIB)(util.o) \
	$(RECLIB)(valptgr.o) \
	$(RECLIB)(vent.o) \
	$(RECLIB)(wglanul.o) \
	$(RECLIB)(wglcalcols.o) \
	$(RECLIB)(wglcont.o) \
	$(RECLIB)(wglfton.o) \
	$(RECLIB)(wgllab.o) \
	$(RECLIB)(wglpfton.o) \
	$(RECLIB)(wglpfton32.o) \
	$(RECLIB)(widgets-util.o) \
	$(RECLIB)(xm-controle.o) 

xrec2000:
	r.build -bidon -main xrec -o xrec -libpath $(REC)/lib/$(ARCH)$(ABI) /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec ezscint wglx Xm Xt X11 m -librmn 

xrec-stereo-linux:
	r.build -bidon -main xrec -o xrec -libpath /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec wglx StaticXm StaticMrm Xp Xt Xmu X11 gd2 z png m -librmn rmnbeta

xrec-stereo:
	r.build -bidon -main xrec -o xrec -libpath /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec wglx Xm Xt X11 gd2 png z m -librmn  rmnbeta

xrec-ezscint:
	r.build -bidon -main xrec -o xrec -libpath $(REC)/lib/$(ARCH)$(ABI) /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec wglx Xm Xt X11 m -obj $(REC)/interp/c_ezscint.o $(REC)/interp/f_ezscint.o -librmn  rmnbeta

xrec-stereo2:
	r.build -bidon -main xrec -o xrec -libpath $(REC)/lib/$(ARCH)$(ABI) /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec wglx Xm Xt X11 m -librmn  rmn_x

xrec:
	r.build -bidon -main xrec -o xrec -libpath $(REC)/lib/$(ARCH)$(ABI) /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec ezscint wglx Xm Xt X11 m -librmn -fstd89

xrec_alpha:
	r.build -bidon -main xrec -o xrec -libpath $(REC)/lib/$(ARCH)$(ABI) /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec wglx Xm Xp Xt Xext X11 gd2 z png m -librmn rmn_x

xreclinux:
	r.build -bidon -main xrec -o xrec -libpath /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec rec_util wgl x_wgl gd_wgl wglx StaticXm StaticMrm Xp Xt Xmu X11 gd2 z png m -librmn rmnbeta

xrecirix:
	r.build -bidon -main xrec -o xrec -libpath /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec rec_util wgl x_wgl gd_wgl Xm Xt X11 gd2 png z m -librmn  rmnbeta

