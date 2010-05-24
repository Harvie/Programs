#!/bin/bash
gcc -Wall -g -o test test.c `pkg-config --cflags --libs gtk+-2.0` -export-dynamic
