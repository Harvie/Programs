#!/bin/sh
./goertzel -f 10 -f 20 -f 40 -f 60 -f 100 -f 200 -f 300 -f 400 -f 500 -f 1000 -f 2000 -f 4000 -f 6100 -f 10000 -f 20000 $@ 2>/dev/null | ./histogram.pl
