.SUFFIXES : 

.SUFFIXES : .ftn .c .o  .a

SHELL = /bin/bash

CPP = /lib/cpp

RECLIB = ../lib/$(EC_ARCH)/librec.a

FFLAGS =  

#CFLAGS = -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I../include -I$(ARMNLIB)/xml/include/libxml2 -I$(HOME)/src/gd2/ -DX_WGL 
CFLAGS = -I../include -DX_WGL  -DMONOCHROME

#OPTIMIZ = -debug -O 0
OPTIMIZ = -O 3

CPPFLAGS = -I../include -DX_WGL

.PRECIOUS:

default: recutil_lib

.c.o:
	s.cc -c $(OPTIMIZ) $(CFLAGS) $<

.c.a:
	s.cc -c $(OPTIMIZ) $(CFLAGS) $<
	ar $(AROPT) $@ $*.o
	rm -f $*.o

CDECKS= \
	 ComputeImage.c  Convolve2D.c  Convolve3D.c  Create.c  Destroy.c  Filters.c \
	 Modify.c        Query.c

FICHIERS = $(CDECKS) 

obj: $(CDECKS)
#Produire les fichiers objets (.o) pour tous les fichiers

clean:
#Faire un petit menage. On enleve tous les .o et les .f \nproduits a partir de .ftn/.ptn
	rm -f *.o


recutil_lib: \
	$(RECLIB)(ComputeImage.o) \
	$(RECLIB)(Convolve2D.o) \
	$(RECLIB)(Convolve3D.o) \
	$(RECLIB)(Create.o) \
	$(RECLIB)(Destroy.o) \
	$(RECLIB)(Filters.o) \
	$(RECLIB)(Modify.o) \
	$(RECLIB)(Query.o) 

