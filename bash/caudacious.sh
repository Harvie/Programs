#!/bin/sh

export XDIALOG_HIGH_DIALOG_COMPAT=true;
DISPLAY=':0.0';
dialog="dialog"; #(dialog|whiptail|Xdialog|gdialog);
tmpfile="/tmp/caudacui-$$.tmp";

#ssh keepalive
while true; do
  sleep 1;
  echo -ne '\x00';
done &

#menu loop
while true; do
	"$dialog" --title "Harvie's Remote Textmode UI for Audacious & amixer" --no-shadow --menu '' 0 0 0\
		' -t' '|>  PLAY/PAUSE'\
		' -s' '##  STOP'\
		' -f' '>> FWD'\
		' -r' '<< REV'\
		' -p' '->  PLAY'\
		' -u' '|| PAUSE'\
		' 100%' '-- VOLUME MAX'\
		' 5%+' '-- VOLUME +5dB'\
		' 5%-' '-- VOLUME -5dB'\
		' 0%- toggle' 'xx MUTE'\
		2>"$tmpfile";
		if [[ "$?" != 0 ]]; then
		  kill "$!"; >/dev/null 2>&1
		  rm -f "$tmpfile";
			exit;
		fi;
		echo 'Processing...';
		if ( grep '%' -F "$tmpfile" ); then
			amixer -c 0 sset Master $(cat "$tmpfile");
			continue;
		fi;
		audacious -n 0 $(cat "$tmpfile");
done;
