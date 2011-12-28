include $(ARMNLIB)/include/$(EC_ARCH)$(ABI)/Makefile_addons

.SUFFIXES : 

.SUFFIXES : .ftn .f .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

RECLIB = $(HOME)/userlibs/$(EC_ARCH)$(ABI)/librec.a

FFLAGS = -I$(ARMNLIB)/include -I../include -DX_WGL

CFLAGS = -I/usr/X11R6/include -I/usr/include/Motif1.2R6 -I$(ARMNLIB)/include -I../include -I$(HOME)/src/gd2/ -DX_WGL

#@OPTIMIZ = -O 3
OPTIMIZ = -debug -O 0

CPPFLAGS = -I$(ARMNLIB)/include =-I../include -DX_WGL

.PRECIOUS: $(RECLIB)

default: reclib

.ftn.o:
	r.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	r.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.c.a:
	r.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.o

.ftn.a:
	r.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<
	ar rv $@ $*.o
	rm -f $*.f $*.o

FTNDECKS= \
	cvtdictrec.ftn defvar.ftn recgrille.ftn rlx.ftn setitem.ftn

CDECKS= \

COMDECKS= \

cvtdictrec:
	r.build -o cvtdictrec -obj *.o  -libpath $(REC)/lib/$(EC_ARCH)$(ABI) /usr/lib/Motif1.2_R6 /usr/lib/X11R6 /usr/X11R6/lib -libappl m -librmn  rmnbeta

