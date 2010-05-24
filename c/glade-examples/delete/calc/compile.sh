#!/bin/bash
gcc -Wall -g -o test test.c `pkg-config --cflags --libs gtk+-2.0` -export-dynamic
cat test.xml | grep -v "requires lib=" > calc.xml
cat test.xml > calc.xml
