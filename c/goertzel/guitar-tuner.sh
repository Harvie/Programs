#!/bin/sh
#Example of guitar tuner using goertzel:
#165Hz = Low E
#How to tune:
#There are 3 frequencies, you have to make the middle one (target frequency) most loud
#and the other two surrounding it shoud show approximately same intensity

arecord -r 44100 | ./goertzel -r 44100 -f 150 -f 165 -f 180 | ./histogram.pl
