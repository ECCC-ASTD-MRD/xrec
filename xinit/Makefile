include Makefile_$(ARCH)$(ABI)

.SUFFIXES : 

.SUFFIXES : .ftn .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

WGLLIB = $(REC)/lib/$(ARCH)$(ABI)/libwglx.a

FFLAGS = -I$(ARMNLIB)/include -I$(REC)/include -DX_WGL

CFLAGS = -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I$(REC)/include -DX_WGL

OPTIMIZ = -debug

CPPFLAGS = -I$(ARMNLIB)/include =-I$(REC)/include -DX_WGL

.PRECIOUS: $(WGLLIB)

default: wgllib

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

CDECKS= \
         xinit.c 

OBJECTS= \
         xinit.o 

COMDECKS= 

obj: $(FTNDECKS) $(FDECKS) $(CDECKS) $(OBJECTS) 
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

