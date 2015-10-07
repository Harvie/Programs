#!/bin/bash

scan_tty() {
	port="$1"
	rate="$2"
	timeout 1 cat < "$port" >/dev/null;
	stty -F "$port" "$rate" cs8 cread clocal
	timeout 1 cat < "$port" >/dev/null;
	(
		banner="$(timeout 7 cat < "$port" | tr '\r\n[:cntrl:]' '...' | grep -i '[a-z0-9]' )"
		[ -n "$banner" ] && echo -e "$port $rate:\t$banner"
		#echo "$port:$rate:$banner"
	) &
	stty -F "$port" "$rate" cs8 cread clocal
	sleep 1
	#Windows newlines are fore HP ProCurve Switches...
	#echo -ne '\x03\r\n\x04\x03\r\n\r\n\r\n' > "$port"
	echo -ne '\x03\r\n\x04\x03' > "$port"
	sleep 3
	echo -ne '\r\n\r\n\r\n' > "$port"
	wait
}

printf '\033[?7l'
for port in /dev/ttyUSB*; do
	for rate in 115200 38400; do
		#echo -ne "\r$port:$rate:     "
		banner="$(scan_tty "$port" "$rate")";
		[ -n "$banner" ] && echo "$banner";
		echo "$banner" | egrep -i 'linux|root|login|password|user|connect|invalid' >/dev/null && break;
	done &
done;
wait
printf '\033[?7h'
