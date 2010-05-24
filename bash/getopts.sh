#!/bin/sh

while getopts "a:b:c:d:" OPT; do
	echo "$OPT ==> $OPTARG";
done

shift $(($OPTIND -1)); echo "Rest: $@";
