include Makefile_$(ARCH)$(ABI)

.SUFFIXES : 

.SUFFIXES : .ftn .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

REC_UTILLIB = $(REC)/lib/$(ARCH)$(ABI)/librec_util.a

FFLAGS = 

CFLAGS = -I$(ARMNLIB)/include -I../include -DX_REC_UTIL

OPTIMIZ = -O 2
OPTIMIZ = -O 0  -debug

CPPFLAGS = -I$(ARMNLIB)/include =-I../include -DX_REC_UTIL

.PRECIOUS:

default: rec_utillib

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


rec_utillib: \
	$(REC_UTILLIB)(xopget.o) \
	$(REC_UTILLIB)(xopset.o) \
	$(REC_UTILLIB)(init.o) \
	$(REC_UTILLIB)(vcar.o) \
	$(REC_UTILLIB)(xplot88.o) 
