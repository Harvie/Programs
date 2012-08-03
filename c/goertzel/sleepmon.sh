#!/bin/bash
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
graphout="${out%%.*}.png"

killall speaker-test &>/dev/null
speaker-test -t sine &>/dev/null &

touch "$out"
bash ./sleepstats.sh "$out" &>/dev/null &

tresh=10
lastdate="$(date +%s)"
screen=false
graph=false

export LC_ALL=C

#trap 'kill -9 $(jobs -p);' SIGINT

while getopts "sg" OPT; do
	test "$OPT" == 's' && screen=true;
	test "$OPT" == 'g' && graph=true;
done

echo "Writing log to: $out";
$graph && echo "Writing graph to: $graphout"
echo
arecord | ./goertzel -n i -q -l c -t $tresh -d 4 | while read line; do
	date="$(date +%s)"
	time="$(echo "$line" | cut -f 1)"
	level="$(echo "$line" | cut -f 2)"
	test "$level" -gt "$tresh" && state=false || state=true
	$state && statenum=1 || statenum=0;
	$state && statename='MOTION!' || statename='Nothing';

	printf  "%.2f\t %s %s %d" "$time" "$date" "$(date '+%F %T')" "$statenum"

	#Debug
	after=$(( $date - $lastdate))
	printf " (%s %3d After %4d secs)\n" "$statename" "$level" "$after";

	#Fun with values
	$state && {
		$screen && xset dpms force off || true;
	} || {
		$screen && xset dpms force on;
	}
	$graph && ./sleepplot.sh "$out" "$graphout" &>/dev/null &

	#Prepare invariants for next round
	lastdate="$date";
done | tee "$out"
kill $(jobs -p); sleep 0.2
echo
echo "Your log: $out"
$graph && {
	./sleepplot.sh "$out" "$graphout" &>/dev/null
	echo "Your graph: $graphout"
}
