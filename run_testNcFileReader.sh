#!/bin/sh

for i in cdl/* ; do
    echo "testing $i"
    ncgen -b -o test.nc $i
    if ! ./testNcFileReader  ; then
        exit 1;
    fi
done
