include /usr/local/env/armnlib/include/$(ARCH)$(ABI)/Makefile_addons

.SUFFIXES : 

.SUFFIXES : .ftn .c .o .a 

SHELL = /bin/sh

CPP = /lib/cpp

REC_UTILLIB = ../lib/lib$(EC_ARCH)$(ABI)_rec_util.a

FFLAGS = -I$(ARMNLIB)/include -I../include -DX_WGL

CFLAGS = -I/opt/include -I/usr/X11R6/include -I$(ARMNLIB)/include -I../include -DX_WGL

OPTIMIZ = -O 2
#OPTIMIZ = -debug -O 0 -optc =-qinfo=all
#OPTIMIZ = -debug -O 0  -optc=-Wall
OPTIMIZ = -debug -O 0  

CPPFLAGS = -I$(ARMNLIB)/include =-I../include -DX_WGL

.PRECIOUS: $(REC_UTILLIB)

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

FTNDECKS=

FDECKS=

CDECKS=  edit.c outil.c repertoire.c select.c

COMDECKS= 

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

rec_utillib:	\
	$(REC_UTILLIB)(edit.o) \
	$(REC_UTILLIB)(outil.o)\
	$(REC_UTILLIB)(repertoire.o) \
	$(REC_UTILLIB)(select.o)

