#!/bin/bash
#Simple script to create ArchLinux system suitable for chroot or virtualserver.
#It will take ~400-500MB but you will probably want to uninstall some pkgs.
#You will be automaticaly chrooted into new system once installation is done...

pacman='/usr/bin/pacman'
ROOT="$1"
pacman="$pacman --noconfirm --ignore kernel26 --root $ROOT"

[ -z "$1" ] && echo "Usage: root@localhost# $0 /path/to/empty/installation/directory/" && exit 1
[ "$UID" != 0 ] && echo "You have to be root!" && exit 7
[ ! -d "$ROOT" ] && echo "Error: $ROOT is not a directory!" && exit 2
[ -n "$(ls -A "$ROOT")" ] && echo "Error: $ROOT is not empty!" && exit 3

echo -e "Hello. I will install ArchLinux to $ROOT directory.\n"
echo '==> Creating pacman DB...'
	mkdir -p "$ROOT/var/lib/pacman/" || exit 4
echo '==> Synchronizing pacman DB...'
	$pacman -Sy || exit 5
echo '==> Installing packages from base group...'
	$pacman -S base || exit 6
echo '==> Minimal configuration - copying from parent...'
	no_blanks() { grep -Ev '^(#|$)'; }
	for i in /etc/resolv.conf /etc/pacman.d/mirrorlist; do
		echo " -> $i";
		cat "$i" | no_blanks >> "$ROOT$i";
	done;
	#Hack for special configurations:
	echo " -> /etc/pacman.d/mirrorlist*";
	cat /etc/pacman.d/mirrorlist* | no_blanks >> "$ROOT/etc/pacman.d/mirrorlist";

echo '==> Entering chroot...'
	chroot $ROOT
echo -e "\nBye bye. You can enter your system again by executing following command as root
chroot '$ROOT'"

exit 0
