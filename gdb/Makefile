.SUFFIXES : 

.SUFFIXES : .ftn .c .o .a 

SHELL = /bin/bash

CPP = /lib/cpp

REC_UTILLIB = ../lib/$(EC_ARCH)/libgdb.a

FFLAGS =  

#CFLAGS = -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I../include -I$(ARMNLIB)/xml/include/libxml2 -I$(HOME)/src/gd2/ -DX_WGL  
CFLAGS = -I../include -DX_WGL -DLinux -DSOCKADDR

#OPTIMIZ = -debug -O 0
OPTIMIZ = -O 2

CPPFLAGS = -I../include -DX_WGL

.PRECIOUS:

default: recutil_lib

.ftn.o:
	s.compile $(OPTIMIZ) -src $<

.c.o:
	s.cc -c  $(OPTIMIZ) $(CFLAGS) $<

.c.a:
	s.cc -c  $(OPTIMIZ) $(CFLAGS) $<
	ar $(AROPT) $@ $*.o
	rm -f $*.o

.ftn.a:
	s.compile $(OPTIMIZ) -src $<
	ar $(AROPT) $@ $*.o
	rm -f $*.o

FTNDECKS= \
         read-latlon.ftn       test-geo.ftn

CDECKS= \
         dbinit.c desc.c dlb.c gdb.c map.c pln.c point.c pol.c rgis.c scnio.c segment.c sock.c

HDECKS= \
         dbinit.h declare.h desc.h dlb.h gdb.h map.h pln.h point.h pol.h rgis.h scnio.h segment.h sock.h swp.h

FICHIERS = $(CDECKS) 

obj: $(CDECKS)
#Produire les fichiers objets (.o) pour tous les fichiers

clean:
#Faire un petit menage. On enleve tous les .o et les .f \nproduits a partir de .ftn/.ptn
	rm -f *.o


recutil_lib: \
	$(REC_UTILLIB)(dbinit.o) \
	$(REC_UTILLIB)(desc.o) \
	$(REC_UTILLIB)(dlb.o) \
	$(REC_UTILLIB)(gdb.o) \
	$(REC_UTILLIB)(map.o) \
	$(REC_UTILLIB)(pln.o) \
	$(REC_UTILLIB)(point.o) \
	$(REC_UTILLIB)(pol.o) \
	$(REC_UTILLIB)(rgis.o) \
	$(REC_UTILLIB)(scnio.o) \
	$(REC_UTILLIB)(segment.o) \
	$(REC_UTILLIB)(sock.o) \
	$(REC_UTILLIB)(swp.o) 

