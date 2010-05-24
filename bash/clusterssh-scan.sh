#!/bin/bash

scan_for_ssh() {
	for i in $( seq 158 255 ); do
		# echo 1 "$1"."$i" 22;
		nc -z -w 1 "$1"."$i" 22;
		if [ "$?" = 0 ]; then
			echo -n "$1.$i ";
		fi;
	done;
	echo;
}

get_ssh_logins() {
	for i in $( scan_for_ssh "$2" ); do
		echo -n "$1"@"$i ";
	done;
	echo;
}

get_ip_range() {
	for i in $( seq 158 255 ); do
		echo -n "$1.$i ";
	done;
	echo;
}

#cssh $( get_ssh_logins root 192.168.2 )

cssh -T "Klub SSPS" root@192.168.2.161
