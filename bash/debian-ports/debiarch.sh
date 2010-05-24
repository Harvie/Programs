#!/bin/bash
dscurl='http://ftp.de.debian.org/debian/pool/main/s/sash/sash_3.7-8.dsc';
pkgurl=$(dirname "$dscurl");

#init
tmpdir="/tmp/debiarch-pid$$";
rm -rf "$tmpdir";
mkdir "$tmpdir"

dscfile="${tmpdir}/desc.dsc";
wget "$dscurl" -O "$dscfile" > /dev/null 2>&1 || exit 1;

echo '# Contributor: Your Name youremail at domain dot com'
echo '# This PKGBUILD was automatically generated and it will probably need few manual fixes'
echo

pkgname=$(cat "$dscfile" | grep '^Binary: ' | cut -d ' ' -f 2 | head -n 1)
echo "pkgname=$pkgname"
pkgver=$(cat "$dscfile" | grep '^Version: ' | cut -d ' ' -f 2 | head -n 1)
echo "pkgver=$pkgver"
stdver=$(cat "$dscfile" | grep '^Standards-Version: ' | cut -d ' ' -f 2 | head -n 1)

#misc
echo 'pkgrel=1'
echo "pkgdesc='Port of Debian package $pkgname to ArchLinux'"
echo "arch=('i686' 'x86_64')"
echo "url='$dscurl'"
echo "license=('GPL')"

echo '#ArchLinux packages will probably have different names to Debian packages:'
echo '#depends=() #check ${srcdir}/${pkgname}-${pkgver}/control for info about depends'
echo '#makedepends=(' $(cat "$dscfile" | grep '^Build-Depends\(\|-Indep\): ' | cut -d ' ' -f 2- ) ')'
echo '#install=debian.install #uncomment if needed'

echo 'source=('
	cat "$dscfile" | grep '^ ' | cut -d ' ' -f 4 | sort -u | while read line; do echo -ne '\t'; echo "${pkgurl}/$line"; done;
echo ')'

echo 'md5sums=('
echo -e '\t#Make your own using md5sum *'
echo ')'


echo '
build() {
	srcsubdir=$(ls -1 --group-directories-first "$srcdir" | head -n 1) #guess it... ;)

	echo -e "\n-------------------------------------------"
	echo "==> Note you can check ${srcdir}/${srcsubdir}/debian/control for more info and run-time dependencies"
	echo "==> and ${srcdir}/${srcsubdir}/debian/README.debian for even more info"
	echo "==> Short version follows:"
	cat  "${srcdir}/${srcsubdir}/debian/control" | grep -i "^\(Depends\|Conflicts\|Suggests\|Description\):" | sort -u
	echo -e "-------------------------------------------\n"

	cd "${srcdir}"

	#Apply debian patch (which contains more patches lol)
	#patch -p0 < ${pkgname}_${pkgver}.diff
	patch -p0 < ${pkgname}*.diff #another ugly asterisk hack muhwahwa...

	#cd ${srcdir}/${pkgname}-${pkgver}/
	cd "${srcdir}/${srcsubdir}/"

	#Apply Debian patches (if any ;o)
	for i in $(cat ./debian/patches/00list); do
		bash "./debian/patches/$i.dpatch" -patch
	done;

	#Debian install script (if any...)
	cp ./debian/postinst "${startdir}/debian.install"

	#real build() work here (you will probably need to edit this):
  cd "${srcdir}/${srcsubdir}" || return 1
  ./configure --prefix=/usr
  make || return 1
  make DESTDIR=$pkgdir install || return 1
}
'

#cleanup
rm -rf "$tmpdir";
