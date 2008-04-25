include /usr/local/env/armnlib/include/$(ARCH)$(ABI)/Makefile_addons

.SUFFIXES : 

.SUFFIXES : .ftn .c .o .a 

SHELL = /bin/sh

CPP = /lib/cpp

REC_UTILLIB = ../lib/$(EC_ARCH)$(ABI)/librec_util.a

FFLAGS =  

#CFLAGS = -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I../include -I$(ARMNLIB)/xml/include/libxml2 -I$(HOME)/src/gd2/ -DX_WGL  
CFLAGS = -I/usr/X11R6/include -I$(ARMNLIB)/include -I../include -I$(ARMNLIB)/xml/include/libxml2 -I$(HOME)/src/gd2/ -DX_WGL -DLinux

OPTIMIZ = -debug -O 0
OPTIMIZ = -O 2

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

FTNDECKS= \
         read-latlon.ftn       test-geo.ftn

CDECKS= \
         dbinit.c desc.c dlb.c gdb.c map.c pln.c point.c pol.c rgis.c scnio.c segment.c sock.c

HDECKS= \
         dbinit.h declare.h desc.h dlb.h gdb.h map.h pln.h point.h pol.h rgis.h scnio.h segment.h sock.h swp.h

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
	$(REC_UTILLIB)(sock.o) 

