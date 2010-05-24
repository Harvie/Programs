#!/bin/bash
GWD=\~/stupid
source include.sh

clear

echo box:\~\# vi /etc/passwd
echo box:\~\# su game

while true; do
	echo -n game@box:$GWD \$ ""
	read gcmd
	case "$gcmd" in
		"exit") ./win.sh stupid && exit ;;
		"help") echo Root allowed you to login from his shell... INCOMPETENT IDIOT!! \;D && echo \"abort\" to exit... ;;
	esac

	game_commands "$gcmd"
	shell_commands "$gcmd"
done
