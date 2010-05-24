#!/bin/bash
cd /home/harvie/Work/hhgame

while true; do

	clear
	cat etc/motd
	echo 0 - Start GAME
	echo 1 - Disconnect
	echo
	echo -n ~\> ""

	read o
	case $o in
		0) ./shell.sh ;;
		1) exit ;;
	esac
done
