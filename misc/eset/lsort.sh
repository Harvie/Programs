#!/bin/bash

cat "$1" | while read line; do
	len=$(echo -n "$line" | wc -c);
	echo "$len $line";
done | sort -nr | cut -d ' ' -f 2-
