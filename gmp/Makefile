.SUFFIXES : 

.SUFFIXES : .ftn .c .o .a

SHELL = /bin/sh

CPP = /lib/cpp

REC_UTILLIB = ../lib/$(EC_ARCH)$(ABI)/librec_util.a

FFLAGS =  

#CFLAGS = -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I../include -I$(ARMNLIB)/xml/include/libxml2 -I$(HOME)/src/gd2/ -DX_WGL  -Wall
#CFLAGS = -I/opt/motif-2.3.2/include  -I/usr/X11R6/include -I$(ARMNLIB)/include -I../include -I$(ARMNLIB)/xml/include/libxml2 -I$(HOME)/src/gd2/ -DX_WGL 
CFLAGS = -I../include -I../gdb

#OPTIMIZ = -debug -O 0 
OPTIMIZ = -O 2

CPPFLAGS = -I../include -DX_WGL

.PRECIOUS:

default: recutil_lib

.ftn.o:
	s.compile $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	s.compile $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.c.a:
	s.compile $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<
	ar $(AROPT) $@ $*.o
	rm -f $*.o

.ftn.a:
	s.compile $(OPTIMIZ)  -src $<
	ar $(AROPT) $@ $*.o
	rm -f $*.o

FTNDECKS= \
         read-latlon.ftn       test-geo.ftn

CDECKS= \
         aff_vecs.c       c_gmpdrw.c       c_gmpg2l.c       c_gmpl2g.c \
         c_gmpopt.c       c_gmpset.c       c_gmpzset.c       c_grfll.c \
         c_llfgr.c       clip.c       comp_map.c       copy_map.c \
         free_map.c       gmp_convert.c       gmp_perim.c       gmp_trim.c \
         gmp_verif.c       newgeoitem.c       params_ligne.c \
         points.c       points_turbo.c        read_lalo.c  \
         swapFloats.c       vec_turbo.c       vecteurs.c tgeo.c c_gmpinit.c \
	 lire_geo.c lire_gdb_geo.c lire_rmn_geo.c lire_rmn_latlon.c gmp_gvlatlon.c

FDECKS= \
         read-latlon.f       test-geo.f

HDECKS= \
         gmp.h

FICHIERS = $(FDECKS) $(CDECKS) 

obj: $(FTNDECKS) $(FDECKS) $(CDECKS)
#Produire les fichiers objets (.o) pour tous les fichiers

tgeo.o: tgeo.c 

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
	$(REC_UTILLIB)(c_gmpinit.o) \
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
	$(REC_UTILLIB)(newgeoitem.o) \
	$(REC_UTILLIB)(gmp_gvlatlon.o) \
	$(REC_UTILLIB)(params_ligne.o) \
	$(REC_UTILLIB)(points.o) \
	$(REC_UTILLIB)(points_turbo.o) \
	$(REC_UTILLIB)(read-latlon.o) \
	$(REC_UTILLIB)(read_lalo.o) \
	$(REC_UTILLIB)(swapFloats.o) \
	$(REC_UTILLIB)(vec_turbo.o) \
	$(REC_UTILLIB)(vecteurs.o) \
	$(REC_UTILLIB)(lire_geo.o) \
	$(REC_UTILLIB)(lire_gdb_geo.o) \
	$(REC_UTILLIB)(lire_rmn_latlon.o) \
	$(REC_UTILLIB)(lire_rmn_geo.o)


tgeo:
	r.build -bidon -main tgeo -o tgeo -obj tgeo.o -libpath /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl rec_util rec rec_util wgl x_wgl wgl gdb expat Xm Xt X11 gd2 png z m -librmn rmn_x
