SHELL = /bin/sh

subdirs = autog gmp select util wgl xdash xplot88 xinit
recdir  = main

all: $(subdirs) $(recdir)


wgllib:
	for repertoire in $(subdirs); do  make -C $$repertoire wgllib; done

reclib:
	cd main; make reclib; cd ..; cd selfic; make reclib; cd ..

clean:
	for dir in $(subdirs); do $(MAKE) -C $$dir clean; done

xrec:
	$(MAKE) -C $(recdir) xrec
