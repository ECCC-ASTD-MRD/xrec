.SUFFIXES : 

.SUFFIXES : .ftn .c .o .a 

SHELL = /bin/bash

CPP = /lib/cpp

REC_UTILLIB = ../lib/$(EC_ARCH)/librec_util.a

FFLAGS = -I../include -DX_WGL

CFLAGS = -I../include -DX_WGL

#OPTIMIZ = -debug -O 0 -optc =-qinfo=all
#OPTIMIZ = -debug -O 0  -optc=-Wall
#OPTIMIZ = -debug -O 0  
OPTIMIZ = -O 2

CPPFLAGS = -I../include -DX_WGL

.PRECIOUS: $(REC_UTILLIB)

default: rec_utillib

.ftn.o:
	r.compile $(OPTIMIZ) -src $<

.c.o:
	s.cc -c $(OPTIMIZ) $(CFLAGS) $<

.c.a:
	s.cc -c $(OPTIMIZ) $(CFLAGS) $<
	ar $(AROPT) $@ $*.o
	rm -f $*.o

.ftn.a:
	r.compile $(OPTIMIZ) -src $<
	ar $(AROPT) $@ $*.o
	rm -f $*.o

FTNDECKS=

FDECKS=

CDECKS=  edit.c outil.c repertoire.c select.c

COMDECKS= 

obj: $(FTNDECKS) $(FDECKS) $(CDECKS) 
#Produire les fichiers objets (.o) pour tous les fichiers


clean:
#Faire un petit menage. On enleve tous les .o et les .f \nproduits a partir de .ftn/.ptn
	rm -f *.o

rec_utillib:	\
	$(REC_UTILLIB)(edit.o) \
	$(REC_UTILLIB)(outil.o)\
	$(REC_UTILLIB)(repertoire.o) \
	$(REC_UTILLIB)(select.o)

