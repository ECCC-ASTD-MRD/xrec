.SUFFIXES : 

.SUFFIXES : .ftn .c .o .a

SHELL = /bin/bash

CPP = /lib/cpp

REC_UTILLIB = ../lib/$(EC_ARCH)$(ABI)/librec_util.a

FFLAGS = 

CFLAGS = -I../include -DX_REC_UTIL

#OPTIMIZ = -O 0  -debug
OPTIMIZ = -O 2

CPPFLAGS = -DX_REC_UTIL

.PRECIOUS:

default: rec_utillib

.ftn.o:
	s.compile $(OPTIMIZ) -src $<

.c.o:
	s.cc -c $(OPTIMIZ) $(CFLAGS) $<

.c.a:
	s.cc -c $(OPTIMIZ) $(CFLAGS) $<
	ar $(AROPT) $@ $*.o
	rm -f $*.o

.ftn.a:
	s.compile $(OPTIMIZ) -src $<
	ar $(AROPT) $@ $*.o
	rm -f $*.o

FTNDECKS= \
         xopget.ftn       xopset.ftn

FDECKS= \
         xopget.f       xopset.f

CDECKS= \
         init.c       vcar.c       xplot88.c

FDECKS= \
         xopget.f       xopset.f

COMDECKS= \
         impnone.cdk       libra.cdk

FICHIERS = $(FDECKS) $(CDECKS) $(ASDECKS) $(HDECKS)


xopget.f: xopget.ftn 	impnone.cdk 	libra.cdk 
xopset.f: xopset.ftn 	impnone.cdk 	libra.cdk 

obj: $(FTNDECKS) $(FDECKS) $(CDECKS)
#Produire les fichiers objets (.o) pour tous les fichiers

clean:
#Faire un petit menage. On enleve tous les .o et les .f \nproduits a partir de .ftn/.ptn
	-if [ "*.[fp]tn" != "`echo *.[fp]tn`" ] ; \
	then \
	for i in *.[fp]tn ; \
	do \
	fn=$${i%.[fp]tn} ; \
	rm -f $$fn.f; \
	done \
	fi
	rm -f *.o

libdir:
	mkdir -p ../lib/$(EC_ARCH)

rec_utillib: libdir \
	$(REC_UTILLIB)(xopget.o) \
	$(REC_UTILLIB)(xopset.o) \
	$(REC_UTILLIB)(init.o) \
	$(REC_UTILLIB)(vcar.o) \
	$(REC_UTILLIB)(xplot88.o) 
