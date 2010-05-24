#!/bin/bash
#OpenGameBook engine implemented in BASH
#<~~Harvie 2oo8

title="OpenGameBook - BASH engine";
game="hacker.txt";
room="2";

export XDIALOG_HIGH_DIALOG_COMPAT=true;
dialog="dialog"; #(dialog|whiptail|Xdialog|gdialog)

#Args
if [ -z "$1" ]; then
	echo "Usage: gbook game.txt [room] [dialog_binary]";
	exit;
fi;
if [ -a "$1" ];	then
	game="$1";
	else echo "File not exist!";
fi;
if [ -n "$2" ]; then room="$2"; fi;
if [ -n "$3" ]; then dialog="$3"; fi;

#Functions
#nop() { echo -n; }

dial_escape() {
	#sed "s/'/\\\\'/g" |  sed 's/"/\\"/g' |  sed 's/!/\!/g';
	iconv -c | sed "s/'/\\\\'/g";
}

fline() {
	head "$1" -n "$2" | tail -n 1 | dial_escape;
}

enter_room() {
  line="$1";
  #echo -n "$menu";
  echo -n "$line" | grep -o '\[[^]]*\]' | while read item; do
		mroom="$(echo "$item" | grep -o '[0-9][0-9]*' | head -n 1)";
		echo "'$mroom'"; echo "'$item'";
  done;
  echo '>'; echo "' '";
  echo 'c'; echo 'Cheat!';
}

xcompat() {
	grep -v 'Gtk-'
}

#Init
tmpfile="/tmp/ogb-$$.tmp";
dtmpfile="/tmp/ogbd-$$.tmp";
head="$(fline "$game" 1)";

#Code
"$dialog" --msgbox "$head" 0 0 --backtitle "$title: $game" --title "$game intro";
while true; do
	line="$(fline "$game" "$room")";
  
  menuargs="--backtitle '$title: $game' --title '$game/$room' --menu '#$room: $line' 0 0 10"
	echo "$(echo "$menuargs" && enter_room "$line")" | xargs "$dialog" 2>"$tmpfile";
	set="$?"; seterr=123; #123 in xargs == 1 in command
	if [ "$set" == "$seterr" ]; then
		"$dialog" --backtitle "$title: $game" --title "Exiting $game" --yesno "End? - Konec?" 0 0
		if [ "$?" == 0 ]; then break; fi;
	fi;
	
	if [ "$(cat "$tmpfile" | xcompat)" == "c" ]; then
		 "$dialog" --backtitle "$title: $game" --title "Cheating in $game" --inputbox "Room? - Mistnost?" 0 0 2>"$tmpfile";
		 set="$?";
	fi;

  echo -n "$(cat "$tmpfile" | xcompat)" | grep "[^0-9]" > /dev/null
  if [[ "$?" != 0 && "$set" != "$seterr" && -n "$(cat "$tmpfile" | xcompat)" ]]; then room="$(cat "$tmpfile" | xcompat)"; fi;
done;

clear;
rm -f "$tmpfile";
rm -f "$dtmpfile";
echo "$title";
echo Game "$game" aborted by user in room "$room";
exit;
