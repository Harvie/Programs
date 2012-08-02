#!/bin/bash
seconds="$2"
START=$(head -n 1 "$1" | cut -f 2)
STARTOK=$({ tail -n 1 "$1.counts.$seconds" || echo 0; } | cut -f 1)
STOP=$( tail -n 1 "$1" | cut -f 2)

fuser "$1.counts.$seconds" &>/dev/null && exit 23;

test "$STARTOK" -gt "$START" && START="$STARTOK";
seq $START $STOP | while read MAX; do
	MIN=$(( $MAX - $seconds  ))
	echo -ne "$MAX\t"
	cat "$1" | cut -f 2,5 | grep -v '\s0' | cut -f 1 | egrep $(echo $(seq $MIN $MAX) | tr ' ' '|') | wc -l
done >> "$1.counts.$seconds"
