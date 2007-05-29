include /usr/local/env/armnlib/include/$(ARCH)$(ABI)/Makefile_addons

.SUFFIXES : 

.SUFFIXES : .ftn .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

REC_UTILLIB = ../lib/lib$(EC_ARCH)$(ABI)_rec_util.a

FFLAGS =  

#CFLAGS = -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I../include -I$(ARMNLIB)/xml/include/libxml2 -I$(HOME)/src/gd2/ -DX_WGL  -Wall
CFLAGS = -I/usr/X11R6/include -I$(ARMNLIB)/include -I../include -I$(ARMNLIB)/xml/include/libxml2 -I$(HOME)/src/gd2/ -DX_WGL  -Wall

OPTIMIZ = -debug -O 0
OPTIMIZ = -O 3

CPPFLAGS = -I$(ARMNLIB)/include =-I../include -DX_WGL

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

CDECKS= \
	 ComputeImage.c  Convolve2D.c  Convolve3D.c  Create.c  Destroy.c  Filters.c \
	 Modify.c        Query.c

FICHIERS = $(CDECKS) 

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


recutil_lib: \
	$(REC_UTILLIB)(ComputeImage.o) \
	$(REC_UTILLIB)(Convolve2D.o) \
	$(REC_UTILLIB)(Convolve3D.o) \
	$(REC_UTILLIB)(Create.o) \
	$(REC_UTILLIB)(Destroy.o) \
	$(REC_UTILLIB)(Filters.o) \
	$(REC_UTILLIB)(Modify.o) \
	$(REC_UTILLIB)(Query.o) 

