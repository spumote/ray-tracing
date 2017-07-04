#!/bin/bash
files=$(find * | grep cpp)

#echo "Files : $files"

libpng_flags="-I/usr/local/include/libpng12 -L/usr/local/lib -lpng -I/usr/local/include/png++"

rm run-main
echo "compiling"
time g++ -pthread -O3 -std=c++11 $files -o run-main $libpng_flags
echo "running"
time ./run-main
