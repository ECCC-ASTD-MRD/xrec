SHELL = /bin/sh

subdirs = autog gd_wgl gmp main select selfic util wgl x_wgl xdash xinit xplot88

default: lib

link_makefiles:
	for dir in $(subdirs); do  cd $$dir; Link_Makefiles; cd ..; done

genlib:
	for dir in $(subdirs); do  cd $$dir; make; cd ..; done

clean:
	for dir in $(subdirs); do cd $$dir; /bin/rm -f *.o *.f *~; cd ..; done

xrec:
	cd main; make xrec-$(ARCH); cd ..
