include Makefile_$(ARCH)$(ABI)

.SUFFIXES : 

.SUFFIXES : .ftn .f .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

REC_UTILLIB = $(REC)/lib/$(ARCH)$(ABI)/librec_util.a

FFLAGS = 

CFLAGS = -I/usr/include/Motif1.2R6 -I/usr/X11R6/include -I$(ARMNLIB)/include -I$(HOME)/../yrc/src/include -DX_WGL

#OPTIMIZ = -O 2
OPTIMIZ = -O 0 -debug

CPPFLAGS = -I$(ARMNLIB)/include =-I$(REC)/include -DX_WGL

.PRECIOUS:

default: rec_utillib

.ftn.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

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

FTNDECKS=  new-xfsl.ftn  new-xfsl97.ftn  xfslvoir97.ftn  xvoir97.ftn

FDECKS= new-xfsl.f  new-xfsl97.f  xfslvoir97.f  xvoir97.f

CDECKS= flush.c  langue.c  parent.c  strutil.c  widgets-util.c  xinit.c  xrecsel.c


OBJECTS= new-xfsl.o  new-xfsl97.o  xfslvoir97.o  xvoir97.o \
flush.o  langue.o  parent.o  strutil.o  widgets-util.o  xinit.o  xrecsel.o

COMDECKS= xfsl-voir.cdk   xfsl.cdk

xfsl-voir.o: 	xfsl-voir.ftn xfsl-voir.cdk 
xvoir.o:	xvoir.ftn 


rec_utillib: $(CDECKS) $(FTNDECKS) $(OBJECTS) \
$(REC_UTILLIB)(flush.o) \
$(REC_UTILLIB)(langue.o)\
$(REC_UTILLIB)(new-xfsl.o)\
$(REC_UTILLIB)(new-xfsl97.o)\
$(REC_UTILLIB)(parent.o) \
$(REC_UTILLIB)(strutil.o) \
$(REC_UTILLIB)(widgets-util.o) \
$(REC_UTILLIB)(xfsl-voir.o) \
$(REC_UTILLIB)(xinit.o) \
$(REC_UTILLIB)(xrecsel.o)  \
$(REC_UTILLIB)(xvoir.o) \
$(REC_UTILLIB)(xvoir97.o) \
$(REC_UTILLIB)(xfslvoir97.o)

xvoir: $(OBJECTS)
	r.build -obj $(OBJECTS) -o xvoir -libpath $(REC)/lib/$(ARCH)$(ABI) /usr/lib/Motif1.2_R6 /usr/lib/X11R6	/usr/X11R6/lib -libappl wglx Xm Xt X11 m -librmn

xvoir-alpha: $(OBJECTS)
	r.build -obj $(OBJECTS) -o xvoir -libpath $(REC)/lib/$(ARCH)$(ABI) /usr/lib/Motif1.2_R6 /usr/lib/X11R6	/usr/X11R6/lib -libappl wglx Xm Xp Xt Xext X11 m -librmn rmn_x

clean:
	/bin/rm -f *.f *.o xvoir
