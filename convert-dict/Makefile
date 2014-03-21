.SUFFIXES : 

.SUFFIXES : .ftn .f .c .o 

SHELL = /bin/bash

CPP = /lib/cpp

RMNLIB = rmn_014

FFLAGS = -I../include -DX_WGL

CFLAGS = -I../include -DX_WGL

#@OPTIMIZ = -O 3
OPTIMIZ = -debug -O 0

CPPFLAGS = -I../include -DX_WGL

.PRECIOUS: 

default: cvtdictrec

.c.o:
	s.cc -I../include -c $<

.ftn.o:
	r.compile $(OPTIMIZ) -src $<

.ftn.a:
	r.compile $(OPTIMIZ) -src $<
	ar rv $@ $*.o
	rm -f $*.f $*.o

FTNDECKS= \
	cvtdictrec.ftn defvar.ftn recgrille.ftn rlx.ftn setitem.ftn

CDECKS= \
	dict.c  grille.c

COMDECKS= \

cvtdictrec: dict.o grille.o
	s.compile -o cvtdictrec -src $(FTNDECKS) -obj dict.o grille.o  -libpath ../lib/$(EC_ARCH) -libsys m -librmn  $(RMNLIB) 
	mkdir -p ../bin/$(BASE_ARCH)
	mv cvtdictrec ../bin/$(BASE_ARCH)
clean:
	rm -f *.o *.f
