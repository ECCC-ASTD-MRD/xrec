include $(ARMNLIB)/include/$(EC_ARCH)$(ABI)/Makefile_addons

.SUFFIXES : 

.SUFFIXES : .ftn .ftn90 .c .o .a

SHELL = /bin/sh

CPP = /lib/cpp

REC_UTILLIB = ../lib/$(EC_ARCH)$(ABI)/librec_util.a

FFLAGS = 

CFLAGS =  -I/usr/X11R6/include  -I/opt/xm/include -I$(ARMNLIB)/include -I../include -DX_WGL

OPTIMIZ = -O 0 -debug -optc =-qinfo=all
OPTIMIZ = -O 0 -debug -optc =-qinfo=all
OPTIMIZ = -O 0 -debug -optc =-Wall 
OPTIMIZ = -O 2

CPPFLAGS = -I$(ARMNLIB)/include =-I../include -DX_WGL

.PRECIOUS:

default: rec_utillib

.ftn90.o:
	r.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $*.ftn90

.ftn.o:
	r.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $*.ftn

.c.o:
	r.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.c.a:
	r.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.o

.ftn.a:
	r.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.o

FTNDECKS= \
         new-xfsl97.ftn

FDECKS= \
         new-xfsl97.f

CDECKS= \
         flush.c parent.c strutil.c widgets-util.c xrecsel.c

COMDECKS= \
         xfsl.cdk

obj: $(CDECKS) $(FDECKS) 
#Produire les fichiers objets (.o) pour tous les fichiers


rec_utillib:	\
	$(REC_UTILLIB)(flush.o) \
	$(REC_UTILLIB)(parent.o) \
	$(REC_UTILLIB)(strutil.o) \
	$(REC_UTILLIB)(widgets-util.o) \
	$(REC_UTILLIB)(xrecsel.o) \
	$(REC_UTILLIB)(new-xfsl97.o) 


