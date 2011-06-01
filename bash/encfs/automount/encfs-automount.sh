#!/bin/sh
#	~/.config/fsniper/scripts/encfs-automount.sh
# Quick & dirty script for automounting EncFS USB drives
# TODO:
#  - Unmounting!!!
#
ASKPASS="/usr/lib/git-core/git-gui--askpass"

lock=/tmp/fsniper_encfs.lock
lpid=$(cat "$lock" 2>/dev/null) &&
ps "$lpid" | grep "$lpid" >/dev/null && {
	echo "Another instance of fsniper_encfs is running"
	exit;
}
echo $BASHPID > "$lock";
sleep 2;

echo
echo ==== EncFS automount script for fsniper ====

list_mounts() {
	cat /proc/mounts | cut -d ' ' -f 2
}

list_mounts | while read mount; do
	config="$mount"'/*/.encfs*';
	echo Looking for "$config"
	config="$(echo $config)"
	[ -r "$config" ] && {
		cyphertext="$(dirname "$config")";
		plaintext="$cyphertext".plain
		echo Found config: "$config";
		echo Trying to mount: "$cyphertext to $plaintext";
		list_mounts | grep "$plaintext" >/dev/null && {
			echo Already mounted: "$plaintext"
		} || {
			echo WOOHOO Will mount "$cyphertext to $plaintext"
			"$ASKPASS" "EncFS $cyphertext to $plaintext" | encfs --stdinpass "$cyphertext" "$plaintext"
		}
	}
done
echo

rm "$lock" 2>/dev/null
