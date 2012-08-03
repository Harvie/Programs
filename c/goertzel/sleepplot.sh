#!/bin/bash

test -z "$1" && {
	echo "Usage: $0 sleeplog.txt"
	exit 23
}

#Approximate size of graph
size="$(tail -n 1 "$1" | cut -d . -f 1)"
test $size -gt 3600 && size="$(( $size/10 ))" #For prolonged periods
size="$(( 600 + $size ))"

#Plot it
gnuplot << EOF
set output "$1.png"
set terminal png size $size,300

set title "Sleep motions"
set xlabel "time"
set ylabel "motion"

set yrange [-0.5:3]

set xdata time
set timefmt "%s"
#set format x "%H:%M:%S"

set grid

#set pointsize 0.5
plot "$1" using 2:5 title "Sensor state" with steps,\
"" using 2:(\$6*3) title "Avg last last 10 seconds" with lines,\
"" using 2:(\$7*3) title "Avg last last 30 seconds" with lines,\
"" using 2:(\$8*3) title "Avg last last 120 seconds" with lines\

EOF




