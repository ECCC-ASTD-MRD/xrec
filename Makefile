SHELL = /bin/sh
XRECDIR = $(PWD)

subdirs = autog gdb gmp lic main select selfic wgl x_wgl gd_wgl xdash xinit xplot88

default: genbin 

genbin:
	make libgd
	make genlib
	make xrec

link_makefiles:
	for dir in $(subdirs); do  cd $$dir; Link_Makefiles; cd ..; done

le_test:
	echo 'CWD' $(CWD) 'PWD' $(PWD)

genlib:
	echo $(XRECDIR)
	for dir in $(subdirs); do  cd $$dir; make; cd ..; done

libgd:
	cd gd-2.0; ./configure --prefix $(XRECDIR)/lib --disable-shared;make install-libLTLIBRARIES; mv .libs/libgd.a $(XRECDIR)/lib/lib$(EC_ARCH)_gd.a; make clean; cd ..

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
