include /usr/local/env/armnlib/include/$(ARCH)$(ABI)/Makefile_addons

.SUFFIXES : 

.SUFFIXES : .ftn90 .ftn .f .c .o .a

SHELL = /bin/sh

CPP = /lib/cpp

RECLIB = ../lib/lib$(EC_ARCH)$(ABI)_rec.a
RECLIB = ../lib/lib$(EC_ARCH)$(ABI)_rec.a

FFLAGS = -I$(ARMNLIB)/include -I../include -DX_WGL -O0 -g
FFLAGS = -I$(ARMNLIB)/include -I../include -DX_WGL -O2

#CFLAGS = -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I../include -I$(ARMNLIB)/xml/include/libxml2 -I/usr/include/libxml2 -I$(HOME)/src/images/gd2/ -DX_WGL
CFLAGS = -I/opt/xm/include -I/usr/X11R6/include -I$(ARMNLIB)/include -I../include -I$(HOME)/src/images/gd2/ -DX_WGL

#OPTIMIZ = -O 3 -optc =-Wall
OPTIMIZ = -O 0 -debug
OPTIMIZ = -O 2

CPPFLAGS = -I$(ARMNLIB)/include =-I../include -DX_WGL

.PRECIOUS: $(RECLIB)

default: reclib

.ftn90.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.f90.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.ftn.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -optc "=$(CFLAGS)" -src $<

.c.a:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.o

.ftn.a:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.f $*.o

.ftn90.a:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.f $*.o

FTNDECKS= \
         convpr.ftn       defvar.ftn       drlstl.ftn \
         grid.ftn         ij2xy_warp.ftn90 ij2xy_warp_r.ftn90 \
	 new-xfsl.ftn       readrec.ftn \
         rec.ftn       recgrille.ftn       rlx.ftn       setcoupe.ftn \
	 setitem.ftn       statfld4.ftn       unsat.ftn       wglrfton.ftn

CDECKS= \
         PointerVent.c       aff_coupe.c       anim.c v_anim.c       aux.c \
         bidim.c       bidim_m.c cmap.c       contour.c       couleur.c \
         coupe.c       dict.c       diffmgr.c       enhance.c \
         fenlatlon.c       fermeur.c       fldmgr.c       fleche.c \
         geometrie.c       grid_status.c       grille.c \
         hilo.c       i2rgdint.c       inversion.c \
         lclevent.c       legende.c       logo_rpn.c \
         manipule.c       mapinfo.c       message.c       minmax_mgr.c no_rmnlib.c \
         nocore.c       opRefusee.c       p_anim.c       p_vanim.c p_attr.c \
         p_avert.c       p_contour.c       p_coupe.c       p_serie.c p_diapo.c \
	 p_echelle_legende_couleur.c \
         p_edition.c       p_geo.c       p_minmax.c       p_palette.c p_valeursponctuelles.c p_valeursmanquantes.c \
         p_vent.c       pack.c       parent.c \
         profil.c       ptval.c       rle.c       serie.c sfcpres.c stats.c \
         strutil.c       util.c       valptgr.c       vecteur.c vent.c homelic.c \
         wglanul.c       wglcalcols.c       wglcalcols_m.c       wglcont.c       wglfton.c \
         wgllab.c       wglpfton.c       wglpfton32.c       widgets-util.c \
         serie_coupe_utils.c xm-controle.c  wglrfton_m.c wglpfton32_m.c wglfton_m.c enhance_m.c c_ezsint_mask.c xstream.c


COMDECKS= \
         impnone.cdk       qqqxtrp.cdk

reclib: $(RECLIB)(PointerVent.o) \
	$(RECLIB)(aff_coupe.o) \
	$(RECLIB)(anim.o) \
	$(RECLIB)(aux.o) \
	$(RECLIB)(bidim.o) \
	$(RECLIB)(bidim_m.o) \
	$(RECLIB)(c_ezsint_mask.o) \
	$(RECLIB)(couleur.o) \
	$(RECLIB)(cmap.o) \
	$(RECLIB)(contour.o) \
	$(RECLIB)(convpr.o) \
	$(RECLIB)(couleur.o) \
	$(RECLIB)(coupe.o) \
	$(RECLIB)(defvar.o) \
	$(RECLIB)(dict.o) \
	$(RECLIB)(diffmgr.o) \
	$(RECLIB)(drlstl.o) \
	$(RECLIB)(enhance.o) \
	$(RECLIB)(enhance_m.o)	\
	$(RECLIB)(fenlatlon.o) \
	$(RECLIB)(fermeur.o) \
	$(RECLIB)(fldmgr.o) \
	$(RECLIB)(fleche.o) \
	$(RECLIB)(geometrie.o) \
	$(RECLIB)(grid.o) \
	$(RECLIB)(grid.o) \
	$(RECLIB)(grid_status.o) \
	$(RECLIB)(grille.o) \
	$(RECLIB)(hilo.o) \
	$(RECLIB)(i2rgdint.o) \
	$(RECLIB)(ij2xy_warp.o) \
	$(RECLIB)(ij2xy_warp_r.o) \
	$(RECLIB)(inversion.o) \
	$(RECLIB)(lclevent.o) \
	$(RECLIB)(legende.o) \
	$(RECLIB)(logo_rpn.o) \
	$(RECLIB)(manipule.o) \
	$(RECLIB)(mapinfo.o) \
	$(RECLIB)(message.o) \
	$(RECLIB)(minmax_mgr.o) \
	$(RECLIB)(new-xfsl.o) \
	$(RECLIB)(no_rmnlib.o) \
	$(RECLIB)(nocore.o) \
	$(RECLIB)(opRefusee.o) \
	$(RECLIB)(p_anim.o) \
	$(RECLIB)(p_attr.o) \
	$(RECLIB)(p_avert.o) \
	$(RECLIB)(p_contour.o) \
	$(RECLIB)(p_coupe.o) \
	$(RECLIB)(p_diapo.o) \
	$(RECLIB)(p_echelle_legende_couleur.o) \
	$(RECLIB)(p_edition.o) \
	$(RECLIB)(p_geo.o) \
	$(RECLIB)(p_minmax.o) \
	$(RECLIB)(p_palette.o) \
	$(RECLIB)(p_serie.o) \
	$(RECLIB)(p_vanim.o) \
	$(RECLIB)(p_vent.o) \
	$(RECLIB)(p_valeursponctuelles.o) \
	$(RECLIB)(p_valeursmanquantes.o) \
	$(RECLIB)(pack.o) \
	$(RECLIB)(parent.o) \
	$(RECLIB)(profil.o) \
	$(RECLIB)(ptval.o) \
	$(RECLIB)(readrec.o) \
	$(RECLIB)(rec.o) \
	$(RECLIB)(recgrille.o) \
	$(RECLIB)(rle.o) \
	$(RECLIB)(rlx.o) \
	$(RECLIB)(serie.o) \
	$(RECLIB)(serie_coupe_utils.o) \
	$(RECLIB)(setcoupe.o) \
	$(RECLIB)(setitem.o) \
	$(RECLIB)(sfcpres.o) \
	$(RECLIB)(statfld4.o) \
	$(RECLIB)(stats.o) \
	$(RECLIB)(strutil.o) \
	$(RECLIB)(unsat.o) \
	$(RECLIB)(util.o) \
	$(RECLIB)(v_anim.o) \
	$(RECLIB)(valptgr.o) \
	$(RECLIB)(homelic.o) \
	$(RECLIB)(vecteur.o) \
	$(RECLIB)(vent.o) \
	$(RECLIB)(wglanul.o) \
	$(RECLIB)(wglcalcols.o) \
	$(RECLIB)(wglcalcols_m.o) \
	$(RECLIB)(wglcont.o) \
	$(RECLIB)(wglfton.o) \
	$(RECLIB)(wglfton_m.o) \
	$(RECLIB)(wgllab.o) \
	$(RECLIB)(wglpfton.o) \
	$(RECLIB)(wglpfton32.o) \
	$(RECLIB)(wglpfton32_m.o) \
	$(RECLIB)(wglrfton.o) \
	$(RECLIB)(wglrfton_m.o) \
	$(RECLIB)(widgets-util.o) \
	$(RECLIB)(xm-controle.o) \
        $(RECLIB)(xstream.o)

xrec2000:
	r.build -bidon -main xrec -o xrec -libpath $(REC)/lib/$(ARCH)$(ABI) /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec ezscint wglx Xm Xt X11 m -librmn 

xrec-stereo-linux:
	r.build -bidon -main xrec -o xrec -libpath /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec wglx StaticXm StaticMrm Xp Xt Xmu X11 gd2 z png m -librmn rmnbeta

xrec-stereo:
	r.build -bidon -main xrec -o xrec -libpath /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec wglx Xm Xt X11 gd2 png z m -librmn  rmn_x

xrec-ezscint:
	r.build -bidon -main xrec -o xrec -libpath $(REC)/lib/$(ARCH)$(ABI) /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec wglx Xm Xt X11 m -obj $(REC)/interp/c_ezscint.o $(REC)/interp/f_ezscint.o -librmn  rmnbeta

xrec-stereo2:
	r.build -bidon -main xrec -o xrec -libpath $(REC)/lib/$(ARCH)$(ABI) /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec wglx Xm Xt X11 m -librmn  rmn_x

xrec-IRIX64:
	r.compile -src rec.ftn -o $(TMPDIR)/xrec -libpath ../lib $(ARMNLIB)/lib/$(ARCH) /usr/X11R6/lib -libappl $(EC_ARCH)$(ABI)_rec $(EC_ARCH)$(ABI)_rec_util $(EC_ARCH)$(ABI)_wgl $(EC_ARCH)$(ABI)_gd_wgl $(EC_ARCH)$(ABI)_x_wgl StaticXm StaticMrm Xmu Xt Xext X11 $(EC_ARCH)$(ABI)_gd png z m dl -librmn rmn_009

xrec-AIX:
	r.compile -debug -O  0 -src rec.ftn -o $(TMPDIR)/xrec -libpath /opt/xm/lib/ $(PWD)/../lib $(ARMNLIB)/lib/$(EC_ARCH) /usr/X11R6/lib -libappl $(EC_ARCH)$(ABI)_rec $(EC_ARCH)$(ABI)_rec_util $(EC_ARCH)$(ABI)_wgl $(EC_ARCH)$(ABI)_gd_wgl $(EC_ARCH)$(ABI)_x_wgl Xm Mrm Xmu Xp Xt Xext X11 $(EC_ARCH)$(ABI)_gd z $(EC_ARCH)$(ABI)_png m dl jpeg -librmn rmnbeta

xrec-$(EC_ARCH)$(ABI):
	r.compile -debug -O  0 -src rec.ftn -o $(TMPDIR)/xrec -libpath /opt/xm/lib/ $(PWD)/../lib $(ARMNLIB)/lib/$(EC_ARCH) /usr/X11R6/lib -libappl $(EC_ARCH)$(ABI)_rec $(EC_ARCH)$(ABI)_rec_util $(EC_ARCH)$(ABI)_wgl $(EC_ARCH)$(ABI)_gd_wgl $(EC_ARCH)$(ABI)_x_wgl Xm Mrm Xmu Xp Xt Xext Xft Xrender X11 $(EC_ARCH)$(ABI)_gd z $(EC_ARCH)$(ABI)_png m dl jpeg -librmn rmnbeta

xrec-sarge:
	cd main; r.compile -src rec.ftn -o $(TMPDIR)/xrec -libpath $(PWD)/../lib $(ARMNLIB)/lib/$(EC_ARCH) /usr/X11R6/lib -libappl $(EC_ARCH)$(ABI)_rec $(EC_ARCH)$(ABI)_rec_util $(EC_ARCH)$(ABI)_wgl $(EC_ARCH)$(ABI)_gd_wgl $(EC_ARCH)$(ABI)_x_wgl StaticXm StaticMrm Xmu Xp Xt Xext X11 $(EC_ARCH)$(ABI)_gd z $(EC_ARCH)$(ABI)_png m dl -librmn rmn_009

xrec-Darwin:
	r.compile -src rec.ftn $(OPTIMIZ); r.build -obj rec.o -o xrec -libpath $(REC)/lib/$(ARCH)$(ABI) $(HOME)/userlibs/$(ARCH) /usr/OpenMotif/lib /usr/X11R6/lib /sw/lib -libappl rec rec_util lic wgl gd_wgl x_wgl Xm Xt X11 xml2 gd m -librmn

xrec-Linux-intel:
	ifort *.o -o $(TMPDIR)/xrec -C90 -L$(REC)/lib/$(ARCH) -L/usr/X11R6/lib -lrec -lrec_util -lgdb -lwgl -lgd_wgl -lx_wgl -lwgl -llic $(ARMNLIB)/lib/$(ARCH)/librmn-intel.a  -lMrm -lXp -lXext -lX11 -lxml2 -lexpat -lgd2 -lz -lpng -lm -ldl -lc 

xrec-intel2:
	ifc *.o -o xrec-intel -i_dynamic -L$(REC)/lib/$(ARCH) -L/opt/old-libc -L/usr/X11R6/lib -L/opt/intel/compiler70/ia32/lib/ -lrec -lrec_util -lgdb -lwgl -lx_wgl -lgd_wgl -lwgl $(ARMNLIB)/lib/librmn-intel.a  -lMrm -lXp -lXext -lX11 -lxml2 -lexpat -lgd2 -lz -lpng -lm -ldl -lPEPCF90

dictxml: dictxml.o test-dictxml.o
	r.build -conly -o dictxml -obj test-dictxml.o dictxml.o -librmn rmn_006 -libappl xml2 m z
