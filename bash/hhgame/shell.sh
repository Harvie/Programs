#!/bin/bash
GWD=\~
source include.sh

clear
while true; do

	echo -n game@box:$GWD \$ ""
	read gcmd
	case "$gcmd" in
		"game") echo You must specify game name \(man game\)... && continue ;;
		"game stupid") ./games/stupid.sh && continue ;;
	esac
		shell_commands "$gcmd"

done
