include Makefile_$(ARCH)$(ABI)

.SUFFIXES : 

.SUFFIXES : .ftn .f .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

REC_UTILLIB = $(REC)/lib/$(ARCH)$(ABI)/librec_util.a

FFLAGS = 

CFLAGS =  -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I$(REC)/include -DX_WGL

#OPTIMIZ = -O 2
OPTIMIZ = -O 0 -debug

CPPFLAGS = -I$(ARMNLIB)/include =-I$(REC)/include -DX_WGL

.PRECIOUS:

default: rec_utillib

.ftn.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $*.ftn

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
         new-xfsl97.ftn

FDECKS= \
         new-xfsl97.f

CDECKS= \
         flush.c       langue.c       parent.c \
         strutil.c       widgets-util.c       xinit.c       xrecsel.c

OBJECTS= \
         flush.o       langue.o       new-xfsl97.o \
         parent.o       strutil.o \
         widgets-util.o xinit.o \
         xrecsel.o

COMDECKS= \
         xfsl.cdk

new-xfsl97.o:	new-xfsl97.ftn 

obj: $(CDECKS) $(FDECKS) $(OBJECTS)
#Produire les fichiers objets (.o) pour tous les fichiers


rec_utillib:	$(CDECKS) $(FDECKS) \
	$(REC_UTILLIB)(flush.o) \
	$(REC_UTILLIB)(langue.o) \
	$(REC_UTILLIB)(parent.o) \
	$(REC_UTILLIB)(strutil.o) \
	$(REC_UTILLIB)(widgets-util.o) \
	$(REC_UTILLIB)(xinit.o) \
	$(REC_UTILLIB)(xrecsel.o) \
	$(REC_UTILLIB)(new-xfsl97.o) 

