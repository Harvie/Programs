#!/bin/bash
while true; do
	echo "$(cat $1)" | while read cmd; do
		. getxsock.sh; export XSOCK_PID XSOCK_WID
		echo "$cmd" | bash
	done;
done
