include /usr/local/env/armnlib/include/$(ARCH)$(ABI)/Makefile_addons

.SUFFIXES : 

.SUFFIXES : .ftn .ftn90 .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

REC_UTILLIB = $(HOME)/userlibs/$(ARCH)$(ABI)/librec_util.a

FFLAGS = 

CFLAGS =  -I/usr/X11R6/include -I$(ARMNLIB)/include -I../include -DX_WGL

OPTIMIZ = -O 0 -debug
OPTIMIZ = -O 2

CPPFLAGS = -I$(ARMNLIB)/include =-I../include -DX_WGL

.PRECIOUS:

default: rec_utillib

.ftn90.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $*.ftn90

.ftn.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $*.ftn

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
         new-xfsl97.ftn

FDECKS= \
         new-xfsl97.f

CDECKS= \
         flush.c       langue.c       parent.c \
         strutil.c       widgets-util.c       xinit.c       xrecsel.c

COMDECKS= \
         xfsl.cdk

obj: $(CDECKS) $(FDECKS) 
#Produire les fichiers objets (.o) pour tous les fichiers


rec_utillib:	\
	$(REC_UTILLIB)(flush.o) \
	$(REC_UTILLIB)(langue.o) \
	$(REC_UTILLIB)(parent.o) \
	$(REC_UTILLIB)(strutil.o) \
	$(REC_UTILLIB)(widgets-util.o) \
	$(REC_UTILLIB)(xinit.o) \
	$(REC_UTILLIB)(xrecsel.o) \
	$(REC_UTILLIB)(new-xfsl97.o) 

xvoir-2000: xfsl-xvoir-2000.o xvoir-2000.o
	r.build -bidon -main xvoir2000 -obj *.o -o xvoir-Linux -libpath /usr/X11R6/lib -libappl rec rec_util rec gdb wgl x_wgl gd_wgl wgl StaticXm StaticMrm Xmu Xp Xt Xext X11 xml2 expat gd2 z png m dl -librmn rmnbeta



