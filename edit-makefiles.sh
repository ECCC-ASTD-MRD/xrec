#!/bin/sh -x
for dir in autog gdb gmp main select selfic wgl x_wgl xdash xinit xplot88 
do
    cd $dir
    vi Makefile
    cd ..
done


