#!/bin/bash
dialog=$(which dialog);

get_vbox_pipes() {
	grep 'hostMode="HostPipe"' ~/.VirtualBox/Machines/*/*.xml | sed -e 's/.*path="\([^"]*\)".*/\1/g' | while read i; do
  	for prefix in '' "$HOME/" "$HOME/.VirtualBox/"; do
    	socket="$prefix""$i"
    	test -S "$socket" && {
	      echo "$socket";
   		}
  	done;
	done;
}

tmp="$(mktemp)";
get_vbox_pipes | while read socket; do
	echo -n "$socket"; echo -ne '\0';
	echo -n "${socket##*/}"; echo -ne '\0';
done | xargs -0 "$dialog" --no-shadow --trim --title 'Sockets' --backtitle 'VirtualBox serial terminal' --menu -- "Select the socket" 0 0 0 2>"$tmp" || {
	echo 'ERROR: No vbox sockets found or selected. :-( Bye.';
	exit 1;
}
socket="$(cat "$tmp")";
rm -f "$tmp";

minicom -c on -o -D unix#"$socket";
