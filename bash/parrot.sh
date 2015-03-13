#!/bin/bash
#This is simple audio echoing script
#usefull as virtual parrot or simple HAM repeater controller

file='/tmp/out.wav'
silence_treshold='8%'
silence_timeout='3'
message_timeout='60'
message_delay='1'
message_intro='repeat'
normalize_db='-1'

while :; do
	echo -e "====> RECORDING\t$file, treshold $silence_treshold, timeout $silence_timeout"
	rec -c 1 -r 22050 "$file" \
		silence 1 0 $silence_treshold 1 00:00:$silence_timeout $silence_treshold \
		trim 0 $message_timeout
	echo -e "====> DELAY\t$message_delay seconds"
	sleep $message_delay
	[ -n "$message_intro" ] && {
		echo -e "====> TTS\t$message_intro"
		which espeak && espeak -- "$message_intro"
	}
	echo -e "====> PLAYING\t$file, normalizing to $normalize_db dB"
	play --norm=$normalize_db "$file"
done


