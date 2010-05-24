#!/bin/bash

tinycat() {
	if [ -a "$1" ]; then
		exec 6<"$1";
		while read line <&6; do
			echo "$line";
		done;	
		exec 6<&-;
	else echo 'FNF!' > /dev/stderr
	fi;
}
