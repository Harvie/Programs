#!/bin/bash

export LC_ALL=C

historymax=120;
historylen='10 30 60 90 120'
laststate=0;
lastdate=''
history='';

#statsdone=$(tail -n $historymax "$1".stats | head -n 1 | cut -d ' ' -f 1);

tail -f "$1" --pid=$$ | while read line; do
	date="$(echo "$line" | cut -d ' ' -f 2)"
	#test -n "$statsdone" && test "$date" -le "$statsdone" && continue;
	state="$(echo "$line" | cut -d ' ' -f 5)"
	test -z "$lastdate" && lastdate="$(( $date - 1 ))"

	#echo $line
	#echo $lastdate $date $state

	for i in $(seq $lastdate $(( $date - 1 ))); do
		echo -ne "$i $state\t"
		history=$(echo -n "$laststate$history" | head -c $historymax)
		for len in $historylen; do
			on="$(echo -n ${history::$len} | tr -d 0 | wc -c)"
			on="$(echo "scale=2; $on/$len" | bc)"
			printf " %.2f" "$on"
		done
		echo
	done

	#History
	#after=$(( $date - $lastdate))
	#test $historymax -gt 0 && {
	#}


	#Prepare invariants for next round
	lastdate="$date";
	laststate="$state";
done > "$1.stats"
