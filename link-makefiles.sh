#!/bin/sh -x
for dir in autog gmp select util wgl xdash xplot88 xinit selfic main wgl x_wgl gd_wgl
do
    cd $dir
    Link_Makefiles
    cd ..
done


