#!/bin/bash
ififo="/tmp/$$.osck";
ofifo="/tmp/$$.isck";
mkfifo "$ififo";
mkfifo "$ofifo";

passfifo() {
	while true; do
		cat "%1";
	done 
}

while true; do
	echo -ne 'HTTP/1.1 200 OK\n\n' > "$ofifo";
	passfifo "$ofifo" | nc -l -p 8080 > "$ififo" &
	cat "$ififo" | head -n 1 | cut -d ' ' -f 2 | read line;
	echo "$line";
	echo "$line" > "$ofifo";
	kill "$!";
done;
