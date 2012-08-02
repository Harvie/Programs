#!/bin/bash

test -z "$1" && {
	echo "Usage: $0 sleeplog.txt"
	exit 23
}

#Count:
seconds=60
cat "$1" | cut -f 2,5 | cut -d ' ' -f 1 | grep '1$' | while read line; do
	time=$(echo $line | cut -d ' ' -f 1);
	time=$(( $time + $seconds - $time%$seconds ))
	echo "$time"
done | uniq -c | sed -e 's/^\s*//g' | tr ' ' '\t' > "$1.counts"


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
plot "$1" using 2:5 with steps, "$1.counts" using 2:(\$1/7) with lines

EOF




