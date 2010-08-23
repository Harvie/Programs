#!/bin/bash
pkgbuild='PKGBUILD'
echo "==> Sourcing $pkgbuild ..."
unset pkgname pkgver pkgrel license url options md5sums sha1sums sha256sums sha384sums sha512sums
. PKGBUILD
echo "==> Package: $pkgname ($pkgver-$pkgrel)"
echo "    License: $license, Homepage: $url"

option='updatesums'
proceed=false;

echo "${options[*]}" | grep "!$option" >/dev/null && {
	echo "==> ERROR: $pkgbuild prevents checksum updating using: options=(!$option)"
	echo "==> Aborted!"
	exit
}
echo "${options[*]}" | grep "[^!]*$option" >/dev/null && {
	echo "==> $pkgbuild forces checksum updating using: options=($option)"
	proceed=true;
}
echo "==> Old checksums:"
echo "md5sums=(${md5sums[*]})"

checksums="$(makepkg -p "$pkgbuild" -g)" 2>/dev/null
echo "==> New checksums:"
echo "$checksums";





exit

update_hashes() {
sed -ne '1h;1!H;${;g;s/md5sums=([^)]*)/'$(makepkg -p "$1" -g 2>/dev/null)'/g;p;}' "$1" > "$1.new"
#maybe this code should be changed to replace only first occurence of md5sums array...
mv -f "$1" "$1.bak"
mv -f "$1.new" "$1"
}

update_hashes() {
#sed -ne '1h;1!H;${;g;s/md5sums=([^)]*)/'$(makepkg -p "$1" -g 2>/dev/null)'/g;p;}' "$1" > "$1.new"
sed -ne '1h;1!H;${;g;s/\(md5\|sha\|sha1\|sha224\|sha256\|sha384\|sha512\)sums=([^)]*)/'$(makepkg -p "$1" -g 2>/dev/null)'/1;p;}' "$1" > "$1.new"
mv -f "$1" "$1.bak"
mv -f "$1.new" "$1"
}


sed -i '/^md5sums=([^)]*)/d' PKGBUILD && makepkg -g >> PKGBUILD && makepkg

sed -ne '1h;1!H;${;g;s/md5sums=([^)]*)/'$(makepkg -g 2>/dev/null)'/g;p;}' PKGBUILD


{ rm PKGBUILD; awk '$0 ~ /^md5sums/ {i = 1; system("makepkg -g 2>/dev/null")}; !i {print}; $0 ~ /\)/ {i = 0}' > PKGBUILD; } < PKGBUILD
