#!/bin/bash
altcache='/tmp/pacman-reinstall-all'
pacman -Qsq | while read package; do
	echo "==================== INSTALLING: $package ====================="; sleep 1;
	pacman --noconfirm --cachedir "$altcache" -Sf "$package"
	yes | LANG=C pacman --cachedir "$altcache" -Scc
	echo;
done;
rm -rf "$altcache"
