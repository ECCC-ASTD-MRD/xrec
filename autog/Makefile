include Makefile_$(ARCH)$(ABI)

.SUFFIXES : 

.SUFFIXES : .ftn .f .c .o 

SHELL = /bin/sh

CPP = /lib/cpp

WGLLIB = $(REC)/lib/$(ARCH)$(ABI)/librec_util.a

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
	 xanotat.ftn 	 xautoget.ftn 	 xautolab.ftn 	 xbreak.ftn \
	 xdispla.ftn 	 xebar.ftn 	 xezmxy.ftn 	 xezmy.ftn \
	 xezxy.ftn 	 xezy.ftn 	 xgettk4.ftn 	 xgridal.ftn \
	 xhstgram.ftn 	 xlabmod.ftn 	 xlgrd.ftn 	 xlinrd.ftn \
	 xlnth.ftn 	 xplot80.ftn 	 xseti.ftn 	 xsysdat.ftn \
	 xtick4.ftn 	 xticks.ftn 	 xuliber.ftn


FDECKS= \
	 xanotat.f 	 xautoget.f 	 xautolab.f 	 xbreak.f \
	 xdispla.f 	 xebar.f 	 xezmxy.f 	 xezmy.f \
	 xezxy.f 	 xezy.f 	 xgettk4.f 	 xgridal.f \
	 xhstgram.f 	 xlabmod.f 	 xlgrd.f 	 xlinrd.f \
	 xlnth.f 	 xplot80.f 	 xseti.f 	 xsysdat.f \
	 xtick4.f 	 xticks.f 	 xuliber.f


OBJECTS= \
	 xanotat.o 	 xautoget.o 	 xautolab.o 	 xbreak.o \
	 xdispla.o 	 xebar.o 	 xezmxy.o 	 xezmy.o \
	 xezxy.o 	 xezy.o 	 xgettk4.o 	 xgridal.o \
	 xhstgram.o 	 xlabmod.o 	 xlgrd.o 	 xlinrd.o \
	 xlnth.o 	 xplot80.o 	 xseti.o 	 xsysdat.o \
	 xtick4.o 	 xticks.o 	 xuliber.o


COMDECKS= \
	 impnone.cdk 	 xautog1.cdk 	 xautog2.cdk 	 xautog3.cdk \
	 xautog4.cdk 	 xautog5.cdk 	 xautog6.cdk 	 xcharsz.cdk \
	 xsyspl1.cdk 	 xsyspl2.cdk


FICHIERS = $(FDECKS) $(CDECKS) $(ASDECKS) $(HDECKS)


xanotat.f: xanotat.ftn 	impnone.cdk 	xautog1.cdk 	xautog2.cdk \
	xautog3.cdk 
xautoget.f: xautoget.ftn 	impnone.cdk 	xautog4.cdk 	xautog5.cdk \
	xautog6.cdk 	xcharsz.cdk 
xautolab.f: xautolab.ftn 	impnone.cdk 	xautog3.cdk 	xcharsz.cdk 
xbreak.f: xbreak.ftn 	impnone.cdk 
xdispla.f: xdispla.ftn 	impnone.cdk 	xautog1.cdk 	xautog3.cdk 
xebar.f: xebar.ftn 	impnone.cdk 
xezmxy.f: xezmxy.ftn 	impnone.cdk 	xautog1.cdk 	xautog3.cdk 
xezmy.f: xezmy.ftn 	impnone.cdk 	xautog1.cdk 	xautog3.cdk 
xezxy.f: xezxy.ftn 	impnone.cdk 	xautog1.cdk 	xautog3.cdk 
xezy.f: xezy.ftn 	impnone.cdk 	xautog1.cdk 	xautog3.cdk 
xgettk4.f: xgettk4.ftn 
xgridal.f: xgridal.ftn 
xhstgram.f: xhstgram.ftn 	impnone.cdk 	xautog1.cdk 	xautog3.cdk 
xlabmod.f: xlabmod.ftn 
xlgrd.f: xlgrd.ftn 	impnone.cdk 
xlinrd.f: xlinrd.ftn 	impnone.cdk 
xlnth.f: xlnth.ftn 	impnone.cdk 	xautog1.cdk 	xautog3.cdk 
xplot80.f: xplot80.ftn 	impnone.cdk 	xautog1.cdk 	xautog2.cdk \
	xautog3.cdk 	xautog6.cdk 	xcharsz.cdk 
xseti.f: xseti.ftn 
xsysdat.f: xsysdat.ftn 
xtick4.f: xtick4.ftn 
xticks.f: xticks.ftn 
xuliber.f: xuliber.ftn 


obj: $(CDECKS) $(FDECKS) $(OBJECTS) 
#Produire les fichiers objets (.o) pour tous les fichiers


rec_utillib: $(CDECKS) $(FDECKS) \
	$(WGLLIB)(xanotat.o) \
	$(WGLLIB)(xautoget.o) \
	$(WGLLIB)(xautolab.o) \
	$(WGLLIB)(xbreak.o) \
	$(WGLLIB)(xdispla.o) \
	$(WGLLIB)(xebar.o) \
	$(WGLLIB)(xezmxy.o) \
	$(WGLLIB)(xezmy.o) \
	$(WGLLIB)(xezxy.o) \
	$(WGLLIB)(xezy.o) \
	$(WGLLIB)(xgettk4.o) \
	$(WGLLIB)(xgridal.o) \
	$(WGLLIB)(xhstgram.o) \
	$(WGLLIB)(xlabmod.o) \
	$(WGLLIB)(xlgrd.o) \
	$(WGLLIB)(xlinrd.o) \
	$(WGLLIB)(xlnth.o) \
	$(WGLLIB)(xplot80.o) \
	$(WGLLIB)(xseti.o) \
	$(WGLLIB)(xsysdat.o) \
	$(WGLLIB)(xtick4.o) \
	$(WGLLIB)(xticks.o) \
	$(WGLLIB)(xuliber.o) 
