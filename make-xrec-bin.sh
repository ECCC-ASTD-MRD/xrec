#!/bin/sh -x
./make-xrec-lib.sh
cd main
\rm bidon.o
make bidon.o
make xrec
cd ..

