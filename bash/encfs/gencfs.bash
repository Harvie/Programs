#!/bin/bash
#############################################################
# gEncFS 0.2 (Harvie 2oo8-2oo9)                             #
# BASH & zenity GUI frontend for mounting EncFS filesystems #
#############################################################

zenity="/usr/bin/zenity";
gksu="/usr/bin/gksu";			#path to gksu or gksudo (doesn't matter)
encfs="/usr/bin/encfs";
grep="/bin/grep";
cut="/bin/cut";				#Arch: /bin/cut vs. Debian: /usr/bin/cut
cat="/bin/cat";

cut=$(which cut); #maybe bit insecure - for compatibility


gencfs_getpass() {
	#zenity - insecure way
		#$zenity --entry --hide-text --text "$(echo -ne "Please enter EncFS password for $1\n$2")" --title "gEncFS: Password for $1" 2>/dev/null;
	#gksu/gksudo - secure way (grabs keyboard!)
  	gksu -p -m "$(echo -ne "Please enter EncFS password for $1\n$2")" 2>/dev/null;
}

gencfs_fuses() {
	echo -e "\n\nList of mouted FUSE filesystems:";
	$cat /etc/mtab | $grep "^\(fuse\|encfs\) " | $cut -d ' ' -f 2;
}

gencfs_mount() {
	errout=$(gencfs_getpass "$1" "$3" | $encfs "--extpass=$cat" "$1" "$2")
	if [ $? != 0 ]; then
		$zenity --question --text "$errout\n\nTry again?" --title "gEncFS: Try again?";
		if [ $? == 0 ]; then
			return 1;
		fi;
	fi;
	return 0;
}

gencfs_checkdir() {
	if [ -d "$1" ]; then echo -n; else
		$zenity --error --title "gEncFS: Error!" --text\
			"Directory $1 doesn't exist.\nYou have to create it manualy by encfs command:\n\nencfs [options] rootDir mountPoint [-- [FUSE Mount Options]]";
		exit;
	fi;
}

gencfs_checkdir "$1";
gencfs_checkdir "$2";

$zenity --question --text "Do you want to (re)mount\n$1  to  $2  ?$(gencfs_fuses;)" --title gEncFS
if [ $? != 0 ]; then
	exit;
fi;

fuseout=$(fusermount -u "$2" 2>&1)
if [ $? == 0 ]; then
	fuseout="";
fi;

false;
while [ $? != 0 ]; do
	gencfs_mount "$1" "$2" "$fuseout";
done;
