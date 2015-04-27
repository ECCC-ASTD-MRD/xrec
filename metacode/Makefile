.SUFFIXES:

.SUFFIXES : .ftn .f .c .o .cdk

include version.inc

FFLAGS =

CFLAGS =

OPTIMIZ = -O 2

CPPFLAGS = -I$(ARMNLIB)/include

DEFINE =

LIBRMN = rmn_015.2

PROGRAM = trames

PROGRAM2 = xmetaview

#PROGRAM3 = xmetaview-IRIX64

default: xmetaview

all:
	make raster
	make clean_obj
	make metaview
	make clean_obj

#define pour trames
#DEFINE =  -DUNIX -DRASTER -DC910
raster:
	make DEFINE='-DUNIX -DRASTER -DC910' trames
	make clean_obj

#define pour xmetaview
#DEFINE =  -DUNIX -DRASTER -DC910 -DNATIF

metaview:
	make DEFINE='-DUNIX -DRASTER -DC910 -DNATIF' xmetaview
#	make clean_obj

.ftn.o:
	s.compile $(OPTIMIZ) -opt "=$(FFLAGS)" -defines "=$(DEFINE)" -src $< -ignorempty

.c.o:
	s.compile $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.f.o:
	s.compile $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

FTNDECKS= \
         account.ftn       asc2dsc.ftn       blacklst.ftn      cmpres2.ftn \
         haftpal.ftn       praster.ftn       pushopt.ftn \
         pushpar.ftn       ttputid.ftn       ttrrini.ftn       tttban.ftn \
         tttbpr.ftn        tttcadr.ftn       tttcari.ftn       tttcfrm.ftn \
         tttclip.ftn       tttcntr.ftn       tttcolr.ftn       tttcomp.ftn \
         tttdraw.ftn       tttdsbx.ftn       ttterfi.ftn       tttfill.ftn \
         tttfini.ftn       tttfmid.ftn       tttfntx.ftn       ttthaft.ftn \
         tttline.ftn       tttlkid.ftn       tttmapc.ftn \
         tttndr2.ftn       tttoptn.ftn       tttpati.ftn \
         tttpcar.ftn       tttprcs.ftn       tttpre2.ftn       tttpre3.ftn \
         tttpwi1.ftn       tttpwry.ftn       tttrin.ftn        tttsegm.ftn \
         tttskfr.ftn       ttttone.ftn       tttundr.ftn       tttv1ior.ftn \
         tttvopn.ftn       tttwndw.ftn       tttxtrc.ftn       unitrad.ftn \
         writraw.ftn       tttinit.ftn       show.ftn          stub.ftn

OBJETS= \
         account.o       asc2dsc.o       blacklst.o      cmpres2.o \
         haftpal.o       pushopt.o \
         pushpar.o       ttputid.o       ttrrini.o       tttban.o \
         tttbpr.o        tttcadr.o       tttcari.o       tttcfrm.o \
         tttclip.o       tttcntr.o       tttcolr.o       tttcomp.o \
         tttdraw.o       tttdsbx.o       ttterfi.o       tttfill.o \
         tttfini.o       tttfmid.o       tttfntx.o       ttthaft.o \
         tttline.o       tttlkid.o       tttmapc.o \
         tttndr2.o       tttoptn.o       tttpati.o \
         tttpcar.o       tttprcs.o       tttpre2.o       tttpre3.o \
         tttpwi1.o       tttpwry.o       tttrin.o        tttsegm.o \
         tttskfr.o       ttttone.o       tttundr.o       tttv1ior.o \
         tttvopn.o       tttwndw.o       tttxtrc.o       \
         writraw.o       tttinit.o       show.o          stub.o

COMDECKS= \
         aaaamac.cdk     argum.cdk       bpairp.cdk      carac.cdk \
         com910.cdk      cpress.cdk      docpar.cdk      impnone.cdk \
         inscont.cdk     ivar.cdk        lnkpak.cdk      mapping.cdk \
         mapvar.cdk      mesgid.cdk      pact.cdk        palpat.cdk \
         params.cdk      patrns.cdk      putcar.cdk      pwryp.cdk  \
         segvar.cdk      teldon.cdk 

FICHIERS = $(FTNDECKS) $(COMDECKS)

account.o: account.ftn 	aaaamac.cdk 	impnone.cdk 
asc2dsc.o: asc2dsc.ftn 	aaaamac.cdk 	impnone.cdk 
blacklst.o: blacklst.ftn 	aaaamac.cdk 	impnone.cdk 
cmpres2.o: cmpres2.ftn 	aaaamac.cdk 	argum.cdk 	cpress.cdk \
	impnone.cdk 	pact.cdk        putcar.cdk 
haftpal.o: haftpal.ftn 	aaaamac.cdk 	impnone.cdk 	palpat.cdk \
	params.cdk 
praster.o: praster.ftn 	aaaamac.cdk 	argum.cdk 	cpress.cdk \
	docpar.cdk 	impnone.cdk 	inscont.cdk 	ivar.cdk \
	mapping.cdk 	mapvar.cdk 	mesgid.cdk 	palpat.cdk \
	params.cdk 	segvar.cdk      bpairp.cdk      com910.cdk
pmetaview.o: pmetaview.ftn 	aaaamac.cdk 	argum.cdk 	cpress.cdk \
	docpar.cdk 	impnone.cdk 	inscont.cdk 	ivar.cdk \
	mapping.cdk 	mapvar.cdk 	mesgid.cdk 	palpat.cdk \
	params.cdk 	segvar.cdk      bpairp.cdk      com910.cdk
pushopt.o: pushopt.ftn 	aaaamac.cdk 	impnone.cdk 	params.cdk 
pushpar.o: pushpar.ftn 	aaaamac.cdk 	impnone.cdk 	params.cdk 
ttputid.o: ttputid.ftn 	aaaamac.cdk 	impnone.cdk 	mesgid.cdk 
ttrrini.o: ttrrini.ftn 	aaaamac.cdk 	impnone.cdk 
tttban.o: tttban.ftn 	aaaamac.cdk 	impnone.cdk 	lnkpak.cdk \
	mesgid.cdk 	params.cdk 
tttbpr.o: tttbpr.ftn 	aaaamac.cdk 	bpairp.cdk 	impnone.cdk \
	mapvar.cdk 	segvar.cdk 
tttcadr.o: tttcadr.ftn 	argum.cdk 	impnone.cdk 	ivar.cdk \
	params.cdk      aaaamac.cdk
tttcari.o: tttcari.ftn 	aaaamac.cdk 	carac.cdk 	impnone.cdk 
tttcfrm.o: tttcfrm.ftn 	aaaamac.cdk 	bpairp.cdk 	impnone.cdk \
	params.cdk 	segvar.cdk 
tttclip.o: tttclip.ftn 	aaaamac.cdk 	impnone.cdk 
tttcntr.o: tttcntr.ftn 	aaaamac.cdk 	carac.cdk 	impnone.cdk \
	params.cdk 	pwryp.cdk 
tttcolr.o: tttcolr.ftn 	aaaamac.cdk 	impnone.cdk 	params.cdk 
tttcomp.o: tttcomp.ftn 	aaaamac.cdk 	argum.cdk 	impnone.cdk \
	ivar.cdk 	mesgid.cdk 	params.cdk 
tttdraw.o: tttdraw.ftn 	aaaamac.cdk 	argum.cdk 	impnone.cdk \
	ivar.cdk 	mapping.cdk 	mapvar.cdk 	params.cdk \
	teldon.cdk 
tttdsbx.o: tttdsbx.ftn 	aaaamac.cdk 	carac.cdk 	impnone.cdk \
	params.cdk 	pwryp.cdk 
ttterfi.o: ttterfi.ftn 	aaaamac.cdk 	impnone.cdk 	mapping.cdk \
	mapvar.cdk 	params.cdk 
tttfill.o: tttfill.ftn 	aaaamac.cdk 	argum.cdk 	impnone.cdk \
	palpat.cdk 	params.cdk 	patrns.cdk 
tttfini.o: tttfini.ftn 	cpress.cdk 	impnone.cdk 	lnkpak.cdk \
	params.cdk 
tttfmid.o: tttfmid.ftn 	aaaamac.cdk 	impnone.cdk 	mesgid.cdk \
	params.cdk 
tttfntx.o: tttfntx.ftn 	aaaamac.cdk 	impnone.cdk 	params.cdk 
ttthaft.o: ttthaft.ftn 	impnone.cdk 	ivar.cdk 	mapping.cdk \
	mapvar.cdk 	params.cdk 	patrns.cdk 
tttline.o: tttline.ftn 	aaaamac.cdk 	argum.cdk 	impnone.cdk 
tttlkid.o: tttlkid.ftn 	aaaamac.cdk 	impnone.cdk 	lnkpak.cdk \
	mesgid.cdk 
tttmapc.o: tttmapc.ftn 	aaaamac.cdk 	impnone.cdk 	params.cdk 
tttndr2.o: tttndr2.ftn 	cpress.cdk      putcar.cdk
tttoptn.o: tttoptn.ftn 	aaaamac.cdk 	impnone.cdk 	params.cdk \
	patrns.cdk 	pwryp.cdk 
tttpati.o: tttpati.ftn 	aaaamac.cdk 	impnone.cdk 	params.cdk \
	patrns.cdk 
tttpcar.o: tttpcar.ftn 	aaaamac.cdk 	carac.cdk 	impnone.cdk \
	mapping.cdk 	mapvar.cdk 	params.cdk 	pwryp.cdk 
tttprcs.o: tttprcs.ftn 	aaaamac.cdk 	impnone.cdk 	inscont.cdk \
	params.cdk 
tttpre2.o: tttpre2.ftn 	aaaamac.cdk 	cpress.cdk 	impnone.cdk \
	pact.cdk 
tttpre3.o: tttpre3.ftn 	aaaamac.cdk 	impnone.cdk 
tttpwi1.o: tttpwi1.ftn 	aaaamac.cdk 	impnone.cdk 	mapvar.cdk \
	params.cdk 	pwryp.cdk 
tttpwry.o: tttpwry.ftn 	aaaamac.cdk 	carac.cdk 	impnone.cdk \
	mapping.cdk 	mapvar.cdk 	palpat.cdk 	params.cdk \
	patrns.cdk 	pwryp.cdk 
tttrin.o: tttrin.ftn 	aaaamac.cdk 	impnone.cdk 	params.cdk 
tttsegm.o: tttsegm.ftn 	aaaamac.cdk 	bpairp.cdk 	impnone.cdk \
	mapvar.cdk 	params.cdk 	segvar.cdk 
tttskfr.o: tttskfr.ftn 	aaaamac.cdk 	bpairp.cdk 	impnone.cdk \
	params.cdk 	segvar.cdk 
ttttone.o: ttttone.ftn 	aaaamac.cdk 	com910.cdk 	impnone.cdk \
	ivar.cdk 	palpat.cdk 	params.cdk 	patrns.cdk 
tttundr.o: tttundr.ftn 	aaaamac.cdk 	carac.cdk 	impnone.cdk \
	params.cdk 	pwryp.cdk 
tttv1ior.o: tttv1ior.ftn	aaaamac.cdk 	argum.cdk 	impnone.cdk 
tttvopn.o: tttvopn.ftn 	aaaamac.cdk 	argum.cdk 	impnone.cdk \
	ivar.cdk 
tttwndw.o: tttwndw.ftn 	aaaamac.cdk 	argum.cdk 	impnone.cdk \
	mapping.cdk 	mapvar.cdk 	params.cdk 
tttxtrc.o: tttxtrc.ftn 	aaaamac.cdk 	impnone.cdk 
unitrad.o: unitrad.ftn 	aaaamac.cdk 	impnone.cdk 	mapvar.cdk \
	params.cdk 
writraw.o: writraw.ftn 	aaaamac.cdk 	argum.cdk 	impnone.cdk \
	ivar.cdk 	mesgid.cdk 	params.cdk 
show.o: show.ftn 
stub.o: stub.ftn        aaaamac.cdk     impnone.cdk
tttinit.o: tttinit.ftn  aaaamac.cdk     impnone.cdk     params.cdk \
        patrns.cdk      mapvar.cdk      segvar.cdk      palpat.cdk

$(PROGRAM): $(OBJETS) praster.o
	s.compile -o $(PROGRAM)_$(subst ",,$(TRAMES_VERSION))_$(BASE_ARCH) -obj $(OBJETS) praster.o -librmn $(LIBRMN)

$(PROGRAM2): $(OBJETS) pmetaview.o
	s.f90 -o $(PROGRAM2)_$(subst ",,$(XMETAVIEW_VERSION))_$(BASE_ARCH) $(OBJETS) pmetaview.o -L../libStatic/$(EC_ARCH) -L../lib/$(EC_ARCH) -lwgl -lgd_wgl -lx_wgl -lwgl -l$(LIBRMN) -lXm -lXt -lSM -lICE -lXext -lXp -lX11 -lgd -lz -lpng -lm -ldl 

clean_obj:
	-if [ "*.ftn" != "`echo *.ftn`" ] ; \
	then \
	for i in *.ftn ; \
	do \
	fn=`r.basename $$i '.ftn'`; \
	rm -f $$fn.f; \
	done \
	fi
	rm -f $(OBJETS) praster.o pmetaview.o
	rm -rf .fo

clean:	clean_obj
	-rm -f $(PROGRAM)_$(subst ",,$(TRAMES_VERSION))_$(BASE_ARCH) $(PROGRAM2)_$(subst ",,$(XMETAVIEW_VERSION))_$(BASE_ARCH) 

