SHELL = /bin/sh
XRECDIR = $(PWD)

subdirs = autog gdb gmp lic main select selfic wgl x_wgl gd_wgl xdash xinit xplot88

default: genbin 

genbin:
	make libpng
	make libgd
	make genlib
	make xrec

genlib:
	echo $(XRECDIR)
	for dir in $(subdirs); do  cd $$dir; make; cd ..; done

libgd:
	cd gd2; make clean; make libgd.a; mv libgd.a ../lib/lib$(EC_ARCH)_gd.a; cd ..

libpng:
	cd png; make clean; make libpng.a; mv libpng.a ../lib/lib$(EC_ARCH)_png.a; cd ..

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
