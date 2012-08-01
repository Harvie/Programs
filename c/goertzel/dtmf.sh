#!/bin/bash
#This is sample implementation of DTMF decoder using my C implementation of Goertzel Algorithm
#This is not very efficient or precise, it's just proof of concept
#Note that it's also quite tricky (but not imposible) to detect DTMF using microphone
#You will probably need to tune treshold and mixer settings a bit...
#I've had best results using dtmfdial (Linux software) and two soundcards connected directly using cable
#Usage example: arecord 2>/dev/null | ./dtmf.sh

tresh=10
last='';
./goertzel -n -q -l -r 8000 -d 20 -t $tresh -f 697 -f 770 -f 852 -f 941 -f 1209 -f 1336 -f 1477 -f 1633 $@ | while read line; do
	#echo "$line";

	#Get time
	pos="$(echo "$line" | cut -f 1)";

	#Get values for each tone
	a="$(echo "$line" | cut -f 2)";
	b="$(echo "$line" | cut -f 3)";
	c="$(echo "$line" | cut -f 4)";
	d="$(echo "$line" | cut -f 5)";
	e="$(echo "$line" | cut -f 6)";
	f="$(echo "$line" | cut -f 7)";
	g="$(echo "$line" | cut -f 8)";
	h="$(echo "$line" | cut -f 9)";

	#Compare values to treshold
	test $a -gt $tresh && a=true || a=false
	test $b -gt $tresh && b=true || b=false
	test $c -gt $tresh && c=true || c=false
	test $d -gt $tresh && d=true || d=false
	test $e -gt $tresh && e=true || e=false
	test $f -gt $tresh && f=true || f=false
	test $g -gt $tresh && g=true || g=false
	test $h -gt $tresh && h=true || h=false

	#echo "$pos: $a $b $c $d $e $f $g $h";
	state="$a$b$c$d$e$f$g$h";

	#Test if tones changed since last time
	#echo test "$state" != "$last"
	test "$state" != "$last" && {

		#DTMF Table
		$a && {
			$e && echo -n '1'
			$f && echo -n '2'
			$g && echo -n '3'
			$h && echo -n 'A'
		}
		$b && {
			$e && echo -n '4'
			$f && echo -n '5'
			$g && echo -n '6'
			$h && echo -n 'B'
		}
		$c && {
			$e && echo -n '7'
			$f && echo -n '8'
			$g && echo -n '9'
			$h && echo -n 'C'
		}
		$d && {
			$e && echo -n '*'
			$f && echo -n '0'
			$g && echo    '#'
			$h && echo -n 'D'
		}

	}

	last="$state"

done
