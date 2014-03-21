.SUFFIXES : 

.SUFFIXES : .ftn .c .o .a 

SHELL = /bin/bash

CPP = /lib/cpp

REC_UTILLIB = ../lib/$(EC_ARCH)$(ABI)/librec_util.a

FFLAGS = 

CFLAGS =  -I../include -DX_WGL

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
	 xcfvld.ftn 	 xcurved.ftn 	 xdashbd.ftn 	 xdashd.ftn \
	 xdashget.ftn 	 xdashi.ftn 	 xdashset.ftn 	 xfdvdld.ftn \
	 xfrstd.ftn 	 xkurv1s.ftn 	 xkurv2s.ftn 	 xlastd.ftn \
	 xlined.ftn 	 xvectd.ftn


CDECKS= \
	 unsat-xdash.c


FDECKS= \
	 xcfvld.f 	 xcurved.f 	 xdashbd.f 	 xdashd.f \
	 xdashget.f 	 xdashi.f 	 xdashset.f 	 xfdvdld.f \
	 xfrstd.f 	 xkurv1s.f 	 xkurv2s.f 	 xlastd.f \
	 xlined.f 	 xvectd.f


COMDECKS= \
	 xdashd1.cdk 	 xdashd2.cdk 	 xdashd3.cdk 	 xdsave1.cdk \
	 xdsave2.cdk


HDECKS= \
	 vcar.h


FICHIERS = $(FDECKS) $(CDECKS) $(ASDECKS) $(HDECKS)


xcfvld.f: xcfvld.ftn 	xdashd1.cdk 	xdashd2.cdk 	xdsave1.cdk \
	xdsave2.cdk 
xcurved.f: xcurved.ftn 
xdashbd.f: xdashbd.ftn 	xdashd2.cdk 	xdashd3.cdk 
xdashd.f: xdashd.ftn 	xdashd1.cdk 	xdashd2.cdk 	xdashd3.cdk \
	xdsave2.cdk 
xdashget.f: xdashget.ftn 	xdashd2.cdk 	xdashd3.cdk 
xdashi.f: xdashi.ftn 	xdashd2.cdk 
xdashset.f: xdashset.ftn 	xdashd2.cdk 	xdashd3.cdk 
xfdvdld.f: xfdvdld.ftn 	xdashd2.cdk 
xfrstd.f: xfrstd.ftn 
xkurv1s.f: xkurv1s.ftn 
xkurv2s.f: xkurv2s.ftn 
xlastd.f: xlastd.ftn 
xlined.f: xlined.ftn 
xvectd.f: xvectd.ftn 

clean:
#Faire un petit menage. On enleve tous les .o et les .f \nproduits a partir de .ftn/.ptn
	for i in *.[fp]tn ; \
	do \
	fn=$${i%.[fp]tn} ; \
	rm -f $$fn.f; \
	done ; \
	rm -f *.o ;\

obj: $(CDECKS) $(FDECKS)
#Produire les fichiers objets (.o) pour tous les fichiers

libdir:
	mkdir -p ../lib/$(EC_ARCH)

rec_utillib: libdir \
	$(REC_UTILLIB)(unsat-xdash.o) \
	$(REC_UTILLIB)(xcfvld.o) \
	$(REC_UTILLIB)(xcurved.o) \
	$(REC_UTILLIB)(xdashbd.o) \
	$(REC_UTILLIB)(xdashd.o) \
	$(REC_UTILLIB)(xdashget.o) \
	$(REC_UTILLIB)(xdashi.o) \
	$(REC_UTILLIB)(xdashset.o) \
	$(REC_UTILLIB)(xfdvdld.o) \
	$(REC_UTILLIB)(xfrstd.o) \
	$(REC_UTILLIB)(xkurv1s.o) \
	$(REC_UTILLIB)(xkurv2s.o) \
	$(REC_UTILLIB)(xlastd.o) \
	$(REC_UTILLIB)(xlined.o) \
	$(REC_UTILLIB)(xvectd.o)
