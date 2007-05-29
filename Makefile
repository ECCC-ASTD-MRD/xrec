SHELL = /bin/sh

subdirs = autog gdb gmp lic main select selfic wgl x_wgl gd_wgl xdash xinit xplot88

default: genlib

link_makefiles:
	for dir in $(subdirs); do  cd $$dir; Link_Makefiles; cd ..; done

genlib:
	for dir in $(subdirs); do  cd $$dir; make; cd ..; done

optimiz:
	for dir in $(subdirs); do  cd $$dir; make OPTIMIZ="-O 3"; cd ..; done

prof:
	for dir in $(subdirs); do  cd $$dir; make OPTIMIZ="-prof"; cd ..; done

clean:
	for dir in $(subdirs); do cd $$dir; /bin/rm -f *.o *.f *.f90 *~ *.stb; cd ..; done

touch:
	for dir in $(subdirs); do cd $$dir; touch *.ftn *.c; cd ..; done

xrec:
	cd main; make xrec-$(ARCH); mv $(TMPDIR)/xrec ../xrec-$(ARCH); cd ..
