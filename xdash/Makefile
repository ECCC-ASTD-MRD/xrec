include Makefile_$(ARCH)$(ABI)

.SUFFIXES : 

.SUFFIXES : .ftn .f .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

REC_UTILLIB = $(REC)/lib/$(ARCH)$(ABI)/librec_util.a

FFLAGS = 

CFLAGS =  -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I$(REC)/include -DX_WGL

OPTIMIZ = -O 2

CPPFLAGS = -I$(ARMNLIB)/include =-I$(REC)/include -DX_WGL

.PRECIOUS:

default: rec_utillib

.ftn.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $*.f

.ftn.f:
	rm -f $*.f
	r.ftntof -P $(DEFINE) $< > $*.f
	chmod 444 $*.f

.c.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.f.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.a:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.o

.f.a:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<
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


OBJECTS= \
	 unsat-xdash.o 	 xcfvld.o 	 xcurved.o 	 xdashbd.o \
	 xdashd.o 	 xdashget.o 	 xdashi.o 	 xdashset.o \
	 xfdvdld.o 	 xfrstd.o 	 xkurv1s.o 	 xkurv2s.o \
	 xlastd.o 	 xlined.o 	 xvectd.o


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

qclean:
#Faire un petit menage. On enleve tous les .o et les .f \nproduits a partir de .ftn/.ptn
	- who am i | grep `whoami` > /dev/null ;\
	if [ "$$?" != "0" ] ;\
	then \
	echo "Probleme de personnalites multiples !" ;\
	echo "                     Utiliser 'rlogin -l' plutot que 'su'" ;\
	echo "Multiple personality problem !" ;\
	echo "                     Use 'rlogin -l' instead of 'su'" ;\
	else \
	if [ -s *.[fp]tn ] ; \
	then \
	for i in *.[fp]tn ; \
	do \
	fn=`basename $$i '\.[fp]tn'`; \
	rm -f $$fn.f; \
	done \
	fi;\
	rm *.o ;\
	fi

obj: $(CDECKS) $(FDECKS) $(OBJECTS) 
#Produire les fichiers objets (.o) pour tous les fichiers


rec_utillib: $(CDECKS) $(FDECKS) \
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
