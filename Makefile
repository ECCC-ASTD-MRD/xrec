SHELL = /bin/sh

subdirs = autog gd_wgl gmp main select selfic wgl x_wgl xdash xinit xplot88
subdirs = autog gmp main select selfic wgl x_wgl xdash xinit xplot88

default: genlib

link_makefiles:
	for dir in $(subdirs); do  cd $$dir; Link_Makefiles; cd ..; done

genlib:
	for dir in $(subdirs); do  cd $$dir; make; cd ..; done

clean:
	for dir in $(subdirs); do cd $$dir; /bin/rm -f *.o *.f *~; cd ..; done

xrec:
	cd main; make xrec-$(ARCH); mv $(TMPDIR)/xrec ../xrec-$(ARCH); cd ..
