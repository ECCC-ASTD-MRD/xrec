include Makefile_$(ARCH)$(ABI)

.SUFFIXES : 

.SUFFIXES : .ftn .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

REC_UTILLIB = $(REC)/lib/$(ARCH)$(ABI)/librec_util.a

FFLAGS = 

CFLAGS = -I$(ARMNLIB)/include -I$(REC)/include -DX_WGL

OPTIMIZ = -debug -O 0
#OPTIMIZ = -O 3

CPPFLAGS = -I$(ARMNLIB)/include =-I$(REC)/include -DX_WGL

.PRECIOUS:

default: recutil_lib

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
         read-latlon.ftn       test-geo.ftn

CDECKS= \
         aff_vecs.c       c_gmpdrw.c       c_gmpg2l.c       c_gmpl2g.c \
         c_gmpopt.c       c_gmpset.c       c_gmpzset.c       c_grfll.c \
         c_llfgr.c       clip.c       comp_map.c       copy_map.c \
         free_map.c       gmp_convert.c       gmp_perim.c       gmp_trim.c \
         gmp_verif.c       igd.c       newgeoitem.c       params_ligne.c \
         points.c       points_turbo.c       read_geo.c       read_lalo.c \
         swapFloats.c       tgeo.c       vec_turbo.c       vecteurs.c

FDECKS= \
         read-latlon.f       test-geo.f

OBJECTS= \
         aff_vecs.o       c_gmpdrw.o       c_gmpg2l.o       c_gmpl2g.o \
         c_gmpopt.o       c_gmpset.o       c_gmpzset.o       c_grfll.o \
         c_llfgr.o       clip.o       comp_map.o       copy_map.o \
         free_map.o       gmp_convert.o       gmp_perim.o       gmp_trim.o \
         gmp_verif.o       igd.o       newgeoitem.o       params_ligne.o \
         points.o       points_turbo.o       read_geo.o       read_lalo.o \
         read-latlon.o       swapFloats.o       test-geo.o       \
         tgeo.o       vec_turbo.o       vecteurs.o

HDECKS= \
         gmp.h

FICHIERS = $(FDECKS) $(CDECKS) $(OBJECTS)

FICHIERS = $(FDECKS) $(CDECKS) $(OBJECTS)

obj: $(FTNDECKS) $(FDECKS) $(CDECKS) $(OBJECTS) 
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


recutil_lib: \
	$(REC_UTILLIB)(aff_vecs.o) \
	$(REC_UTILLIB)(c_gmpdrw.o) \
	$(REC_UTILLIB)(c_gmpg2l.o) \
	$(REC_UTILLIB)(c_gmpl2g.o) \
	$(REC_UTILLIB)(c_gmpopt.o) \
	$(REC_UTILLIB)(c_gmpset.o) \
	$(REC_UTILLIB)(c_gmpzset.o) \
	$(REC_UTILLIB)(c_grfll.o) \
	$(REC_UTILLIB)(c_llfgr.o) \
	$(REC_UTILLIB)(clip.o) \
	$(REC_UTILLIB)(comp_map.o) \
	$(REC_UTILLIB)(copy_map.o) \
	$(REC_UTILLIB)(free_map.o) \
	$(REC_UTILLIB)(gmp_convert.o) \
	$(REC_UTILLIB)(gmp_perim.o) \
	$(REC_UTILLIB)(gmp_trim.o) \
	$(REC_UTILLIB)(gmp_verif.o) \
	$(REC_UTILLIB)(igd.o) \
	$(REC_UTILLIB)(newgeoitem.o) \
	$(REC_UTILLIB)(params_ligne.o) \
	$(REC_UTILLIB)(points.o) \
	$(REC_UTILLIB)(points_turbo.o) \
	$(REC_UTILLIB)(read-latlon.o) \
	$(REC_UTILLIB)(read_geo.o) \
	$(REC_UTILLIB)(read_lalo.o) \
	$(REC_UTILLIB)(swapFloats.o) \
	$(REC_UTILLIB)(test-geo.o) \
	$(REC_UTILLIB)(tgeo.o) \
	$(REC_UTILLIB)(vec_turbo.o) \
	$(REC_UTILLIB)(vecteurs.o) 


tgeo:
	f9032 -L. -L/home/dormrb02/util/xrec/lib/Linux -L/usr/lib/Motif1.2_R6 -L/usr/local/env/armnlib/lib/Linux -L/usr/lib/X11R6 -L/usr/X11R6/lib -tp px -fpic -byteswapio -I. -I/usr/local/env/armnlib/include -O1 -o tgeo *.o ../main/grid.o -lrec -lezscint -lwglx -lStaticXm -lStaticMrm -lXp -lXmu -lXt -lX11 -lm -lrmnbeta2

tgeo2:
	r.build -bidon -main tgeo -o tgeo -obj tgeo.o ../main/grid.o  -libpath $(REC)/lib/$(ARCH)$(ABI) /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec wglx Xm Xp Xt Xext X11 m -librmn rmn_x

