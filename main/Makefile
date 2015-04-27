.SUFFIXES :

.SUFFIXES : .ftn90 .ftn .f .c .o .a

SHELL = /bin/bash

CPP = /lib/cpp

RECLIB = ../lib/$(EC_ARCH)$(ABI)/librec.a

#FFLAGS =  -I../include -DX_WGL -O0 -g
FFLAGS = -I../include -DX_WGL -O2

#CFLAGS = -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I../include -I$(ARMNLIB)/xml/include/libxml2 -I/usr/include/libxml2 -I$(HOME)/src/images/gd2/ -DX_WGL
#CFLAGS = -I/usr/X11R6/include -I/home/ordenv/ssm-domains/ssm-tools/motif_2.3.3_linux26-i686/include -I$(ARMNLIB)/include -I../include -I$(ARMNLIB)/xml/include/libxml2 -I/usr/include/libxml2 -I$(HOME)/src/images/gd2/ -DX_WGL
CFLAGS = -I../include -DX_WGL

#OPTIMIZ = -O 0 -debug 
#OPTIMIZ = -O 3 -optc =-Wall
OPTIMIZ = -O 2

CPPFLAGS = -I../include -DX_WGL

LIBRMN = rmn_015.2

include version.inc

.PRECIOUS: $(RECLIB)

default: reclib

.ftn90.o:
	s.compile $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.f90.o:
	s.compile $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.ftn.o:
	s.compile $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	s.cc -c $(OPTIMIZ) $(CFLAGS) $<
#	s.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -includes /opt/motif-2.3.2/include -optc "=$(CFLAGS)" -src $<

.c.a:
	s.cc -c $(OPTIMIZ) $(CFLAGS) $<
	ar rv $@ $*.o
	rm -f $*.o

.ftn.a:
	s.compile $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<
	ar rv $@ $*.o
#	rm -f $*.f $*.o
	rm -f $*.o

.ftn90.a:
	s.compile $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.f $*.o

FTNDECKS= \
         convpr.ftn       defvar.ftn       drlstl_fxfy2.ftn drlstl.ftn \
         grid.ftn         ij2xy_warp.ftn90 ij2xy_warp_r.ftn90 \
	 new-xfsl.ftn       readrec.ftn \
         rec.ftn       recgrille.ftn       rlx.ftn       setcoupe.ftn \
	 setitem.ftn       statfld4.ftn       unsat.ftn       wglrfton.ftn \
	 mapxy2fxfy.ftn90 wglpolyftn.ftn

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
         profil.c       ptval.c       rec_redirect.c rle.c       serie.c sfcpres.c stats.c \
         strutil.c       util.c       valptgr.c       vecteur.c vent.c homelic.c \
         wglanul.c       wglcalcols.c       wglcalcols_m.c       s.c  wglcont_fxfy2.c     wglfton.c \
         wgllab.c       wglpfton.c       wglpfton32.c       widgets-util.c \
         serie_coupe_utils.c xm-controle.c  wglrfton_m.c wglpfton32_m.c wglfton_m.c enhance_m.c c_ezsint_mask.c xstream.c


COMDECKS= \
         impnone.cdk       qqqxtrp.cdk

reclib: $(RECLIB)(PointerVent.o) \
	$(RECLIB)(fst_get_mask_key.o) \
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
	$(RECLIB)(drlstl_fxfy2.o) \
	$(RECLIB)(enhance.o) \
	$(RECLIB)(enhance_m.o)	\
	$(RECLIB)(f_convip.o)	\
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
	$(RECLIB)(rec_redirect.o) \
	$(RECLIB)(recgrille.o) \
	$(RECLIB)(rle.o) \
	$(RECLIB)(rlx.o) \
	$(RECLIB)(serie.o) \
	$(RECLIB)(serie_coupe_utils.o) \
	$(RECLIB)(setcoupe.o) \
	$(RECLIB)(setitem.o) \
	$(RECLIB)(set_stack_size.o) \
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
	$(RECLIB)(wglcont_fxfy2.o) \
	$(RECLIB)(wglfton.o) \
	$(RECLIB)(wglfton_m.o) \
	$(RECLIB)(wgllab.o) \
	$(RECLIB)(wglpfton.o) \
	$(RECLIB)(wglpfton32.o) \
	$(RECLIB)(wglpfton32_m.o) \
	$(RECLIB)(wglrfton.o) \
	$(RECLIB)(wglpolyfton.o) \
	$(RECLIB)(mapxy2fxfy.o) \
	$(RECLIB)(wglrfton_m.o) \
	$(RECLIB)(widgets-util.o) \
	$(RECLIB)(xm-controle.o) \
        $(RECLIB)(xstream.o)

clean:
	-if [ "*.[fp]tn" != "`echo *.[fp]tn`" ] ; \
	then \
	for i in *.[fp]tn ; \
	do \
	fn=$${i%.[fp]tn} ; \
	rm -f $$fn.f; \
	done \
	fi

	rm -f *.o xrec_$(XREC_VERSION)-$(BASE_ARCH)

xrec:
	s.compile -src rec.ftn
	s.f90 -o xrec_$(subst ",,$(XREC_VERSION))_$(BASE_ARCH) rec.o  -L../libStatic/$(EC_ARCH) -L../lib/$(EC_ARCH) -lrec -lrec_util -lwgl -lgd_wgl -lx_wgl -lgdb -l$(LIBRMN) -lXm -lXt -lSM -lICE -lXext -lXp -lX11 -lgd -lz -lpng -lm -ldl -ljpeg
#	mv xrec_$(XREC_VERSION)_$(BASE_ARCH) ..

xrec-dyn:
	s.compile -src rec.ftn
	s.f90 -o xrec rec.o  -L../lib/$(EC_ARCH) -lrec -lrec_util -lwgl -lgd_wgl -lx_wgl -lgdb -l$(LIBRMN) -lXm -lXt -lX11 -lgd -lz -lpng -lm -ldl -ljpeg
	mv xrec ..

xrec-mfv:
	s.compile -src rec.ftn
	s.f90 -o xrec rec.o  -L../lib/$(EC_ARCH) -L../LibStatic -L../lib/$(BASE_ARCH) -lrec -lrec_util -lwgl -lgd_wgl -lx_wgl -lgdb -lrmn_014 -lXm_s -lMrm_s -lXmu -lXp -lXt -lXext -lXft -lXrender -lX11 -lgd -lz -lpng -lm -ldl -ljpeg
	mv xrec ../bin/$(BASE_ARCH)

xrec-AIX:
	s.compile -debug -O  0 -src rec.ftn -o $(TMPDIR)/xrec -obj $(HOME)/src/isi4/lib/ibm/*.o -libpath /opt/xm/lib/ $(PWD)/../lib $(ARMNLIB)/lib/$(EC_ARCH) /usr/X11R6/lib -libappl $(EC_ARCH)$(ABI)_rec $(EC_ARCH)$(ABI)_rec_util $(EC_ARCH)$(ABI)_wgl $(EC_ARCH)$(ABI)_gd_wgl $(EC_ARCH)$(ABI)_x_wgl Xm Mrm Xmu Xp Xt Xext X11 $(EC_ARCH)$(ABI)_gd z $(EC_ARCH)$(ABI)_png m dl jpeg -librmn rmnbeta

dictxml: dictxml.o test-dictxml.o
	r.build -conly -o dictxml -obj test-dictxml.o dictxml.o -librmn rmn_006 -libappl xml2 m z
