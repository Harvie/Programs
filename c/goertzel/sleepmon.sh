#!/bin/sh
# Simple sleep monitor (Harvie 2012)
#
# You probably have soundcard with one output and input
# Take PIR motion sensor from security system
# Use PIR to switch connection between left channels of output and input (tips of 3.5mm jacks)
# Connect grounds of these two together (maybe you will not need it - depending on your soundcard)
# Power up PIR
# Launch this script
# Use alsamixer and some GUI recorder (like audacity) to tune volume to some usable level
# Place PIR facing to your bed and go sleep
# Wake up
# Enjoy your data


out=/tmp/sleeplog-"$(date +%F_%T)".txt
speaker-test -t sine &>/dev/null &
pid_test=$!
tresh=10
lastdate=0
screen=false
while getopts "s" OPT; do
	test "$OPT" == 's' && screen=true;
done
echo "Writing to file: $out";
arecord | ./goertzel -n i -q -l c -t $tresh -d 4 | while read line; do
	date="$(date +%s)"
	time="$(echo "$line" | cut -f 1)"
	level="$(echo "$line" | cut -f 2)"
	echo -ne "$time\t$date\t$(date '+%F%t%T')\t"
	test "$level" -gt "$tresh" && {
		echo -n "0 Nothing detected...";
		$screen && xset dpms force off || true;
	} || {
		echo -n "1 Motion detected!!!!";
		$screen && xset dpms force on;
	}
	test "$lastdate" != 0 && {
		after=$(( $date - $lastdate))
		echo -ne "\t$level After $after secs";
	}
	echo;
	./sleepplot.sh "$out" &>/dev/null
	lastdate="$date";
done | tee "$out"
kill $pid_test
echo
echo "Your file: $out"
