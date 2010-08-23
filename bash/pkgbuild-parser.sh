#!/bin/bash
echo '<?php'
. PKGBUILD

get_function() {
	export -f "$1" || exit 1;
	bash -c 'env' | tr '\n' '\0' | sed 's/.*'"$1"'=() {\(.*\)}.*/\1/g' | tr '\0' '\n'
}

echo '$packages=array('
echo "${pkgname[*]}" | egrep -o '[a-z0-9-]'* | while read pkg; do echo "'$pkg', "; done
echo ');'

echo "${pkgname[*]}" | egrep -o '[a-z0-9-]'* | while read pkg; do
	get_function package_"$pkg" | while read i; do eval "$i" &>/dev/null; echo '$pkgdescs['"'$pkg']='$pkgdesc';"; done | tail -n1
done;

