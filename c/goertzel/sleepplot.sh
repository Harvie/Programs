#!/bin/bash

test -z "$1" && {
	echo "Usage: $0 sleeplog.txt"
	exit 23
}

in="$1";
graphout="${in%%.*}.png"
test -n "$2" && graphout="$2";

#Last state
last="$(tail -n 1 "$in" | cut -d ' ' -f 5)"
test "$last" -gt 0 && last="motion" || last="peace";

#Approximate size of graph
size="$(tail -n 1 "$in" | cut -d . -f 1 | sed -e 's/^0*//' | tee /tmp/lol)"
test $size -gt 3600 && size="$(( $size/10 ))" #For prolonged periods
size="$(( 600 + $size ))"

#Plot it
gnuplot << EOF
set output "$graphout"
set terminal png size $size,400

set title "Sleep motions (current state: $last)"
set xlabel "time"
set ylabel "motion"

set yrange [-0.5:3]

set xdata time
set timefmt "%s"
#set format x "%H:%M:%S"

set grid

#set pointsize 0.5
plot\
"$in" using 2:5 title "Sensor state" with steps,\
"" using 2:((\$6*3+\$7*6+\$8*6+\$9+\$10)/5) title "Avg." with lines\
#"" using 2:(\$6*2) title "Avg. 10s" with lines,\
#"" using 2:(\$7*3) title "Avg. 30s" with lines,\
#"" using 2:(\$8*3) title "Avg. 60s" with lines,\
#"" using 2:(\$9*3) title "Avg. 90s" with lines,\
#"" using 2:(\$10*4) title "Avg. 120s" smooth bezier\


EOF




