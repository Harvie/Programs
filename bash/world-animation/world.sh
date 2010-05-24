#!/bin/bash

while true; do
	cat world.txt | tr ' ' a | while read i; do
		if [[ "$i" =~ World ]]; then
			#sleep 0.2;
			clear;
		else
			echo "$i" | tr a ' ';
		fi;
	done;
done;
