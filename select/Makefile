.SUFFIXES : 

.SUFFIXES : .ftn .ftn90 .c .o .a

SHELL = /bin/bash

CPP = /lib/cpp

REC_UTILLIB = ../lib/$(EC_ARCH)/librec_util.a

FFLAGS = 

CFLAGS =  -I../include -DX_WGL

#OPTIMIZ = -O 0 -debug -optc =-qinfo=all
#OPTIMIZ = -O 0 -debug -optc =-qinfo=all
#OPTIMIZ = -O 0 -debug -optc =-Wall 
OPTIMIZ = -O 2

CPPFLAGS = -I../include -DX_WGL

.PRECIOUS:

default: rec_utillib

.ftn.o:
	s.compile $(OPTIMIZ) -src $<

.c.o:
	s.cc -c $(OPTIMIZ) $(CFLAGS) $<

.c.a:
	s.cc -c $(OPTIMIZ) $(CFLAGS) $<
	ar rv $@ $*.o
	rm -f $*.o

.ftn.a:
	s.compile $(OPTIMIZ) -src $<
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

clean:
	rm -f *.o
