#!/bin/bash

test -z "$1" && {
	echo "Usage: $0 sleeplog.txt"
	exit 23
}

size="$(tail -n 1 "$1" | cut -d . -f 1)"
test $size -gt 3600 && size="$(( $size/10 ))" #For prolonged periods
size="$(( 200 + ($size * 2) ))"

gnuplot << EOF
set output "$1.png"
set terminal png size $size,200

set title "Sleep motions"
set xlabel "time"
set ylabel "motion"

set yrange [-0.5:1.5]

set xdata time
set timefmt "%s"
#set format x "%H:%M:%S"

#set y2tics 300
#set ytics 5

set grid

#set pointsize 0.5
plot "$1" using 2:5 with steps;

EOF




