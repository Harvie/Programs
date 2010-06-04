#!/bin/bash
db="$(mktemp)"
fs="$(mktemp)"
orphans="$(mktemp)"

echo "[*] Saving list of files owned by pacman to $db ..." >&2
time /usr/bin/pacman -Qlq | sort -u > "$db"
echo >&2
echo "[*] Saving list of files in filesystem to $fs ..." >&2
time find /arch /bin /boot /etc /lib /opt /sbin /usr 2>/dev/null | while read i; do
	if [ ! -d "$i" ]; then
		echo "$i"
	fi;
done | sort -u > "$fs"
echo >&2
echo "[*] Comparing db ( $db ) and fs ( $fs ) ..." >&2
time comm -23 "$fs" "$db" | tee "$orphans"
rm -- "$db" "$fs"
echo >&2
echo "[!] List of files not owned by any package were saved to: $orphans" >&2

#test it:
#cat "$orphans" | while read i; do pacman -Qo "$i" 2>&1; done | tee /tmp/orphantest.txt
