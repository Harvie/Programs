#!/bin/sh
#env; exit 0

jail_dir="/var/chroot-jails"
jail_system="/var/chroot-sys"

user="$PAM_USER"
home="$(getent passwd "$user" | cut -f6 -d:)"
home_system="$jail_system/$home"
home_chroot="$jail_dir/$user/$home";

echo "$PAM_USER, $PAM_SERVICE, $PAM_TTY, $PAM_RHOST, $PAM_TYPE"

[ "$user" = "root" ] && {
	echo "no chroots for roots"
	exit 0;
}

#TODO: test if $home is in /home/

mkdir -p "$home_system"
mkdir -p "$jail_dir/$user"
chown "$user:$user" "$home_system"
chown -R root:root "$jail_system"

function is_mounted() {
	dir=$(echo "$1" | sed -e 's/\/\/*/\//g; s/\/$//g;')
	cut -d ' ' -f 2 /proc/mounts  | grep "^$dir$" >/dev/null
}

function bind() {
	from="$1"
	to="$2"
	opt="$3"

	is_mounted "$to" || {
		echo "Mounting: $from to $to";
		mount -o bind "$from" "$to"
	}
	[ -n "$opt" ] && mount -o remount,bind,"$opt" "$to"
	is_mounted "$to" || {
		echo "Not mounted: $to"
		return 1
	}
	return 0
}

case "$PAM_TYPE" in
	close_session)
		#TODO: unmount if not busy
		#umount "$jail_dir/$user" && umount "$home_chroot"
		exit 0
		;;
	*)
		bind "$jail_system" "$jail_dir/$user" ',ro' || exit 1
		bind "$home" "$home_chroot" || exit 2
		;;
esac

exit 0
