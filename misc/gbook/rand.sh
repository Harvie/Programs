#!/bin/bash

getline() {
	head -n "$1" | tail -n 1;
}

randline() {
	while read line; do echo $((len+=1)) >/dev/null; echo "$line"; done;
	echo "$(( ($RANDOM % $len) + 1 ))" >&2;
}


ls | randline
