#!/bin/bash
#Random IP scanner
PORT=22;

function rand_addr() {
	echo $[$RANDOM%255].$[$RANDOM%255].$[$RANDOM%255].$[$RANDOM%255];
}

echo Scanning random IPs for port $PORT opened...
while true; do
	RADDR=$(rand_addr);
	echo -n $RADDR:$PORT:;
	nc -z -w 1 $RADDR $PORT > /dev/null 2>&1;
	case "$?" in
		"0") echo open; echo $RADDR >> open$(echo $PORT)s.txt ;;
		*)   echo closed
	esac;
done;
