.SUFFIXES : 

.SUFFIXES : .ftn .c .o .a 

SHELL = /bin/sh

CPP = /lib/cpp

WGLLIB = ../lib/$(EC_ARCH)$(ABI)/libx_wgl.a

FFLAGS = -I$(ARMNLIB)/include -I../include -DX_WGL

CFLAGS = -I/usr/X11R6/include -I$(ARMNLIB)/include -I../include -DX_WGL


OPTIMIZ = -O 0 -debug 
OPTIMIZ = -O 2

CPPFLAGS = -I$(ARMNLIB)/include =-I../include -DX_WGL

.PRECIOUS: $(WGLLIB)

default: wgllib

.ftn.o:
	s.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	s.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.c.a:
	s.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.o

.ftn.a:
	s.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.o

FTNDECKS= 

FDECKS= 

CDECKS= \
         xinit.c 

OBJECTS = \
         xinit.o 

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

wgllib: $(WGLLIB)(xinit.o)

