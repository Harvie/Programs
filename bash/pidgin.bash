#!/bin/bash
#
# 01pidgin: suspend/wakeup pidgin

purple_remote='/usr/bin/purple-remote';

purple_global_remote() {
	for user in $(ps -o euser,comm -C dbus-daemon | tail -n +2 | cut -d ' ' -f 1 | grep -v '^dbus$'); do
		command="$1";
		tempfile="/tmp/pidgin-last-status-$user";
		if [ "$command" == 'resume' ]; then
			command='setstatus?status='$(cat "$tempfile");
		else
			last_status=$(su "$user" -c "$purple_remote getstatus");
			echo -n "$last_status" > "$tempfile";
			echo "user $user will be resumed to: $last_status";
		fi;
		su "$user" -c "$purple_remote $command";
	done;
}

case "$1" in
	hibernate|suspend)
		purple_global_remote 'setstatus?status=offline';
		sleep 1;
		;;
	thaw|resume)
		purple_global_remote resume;
		;;
	*) exit $NA
		;;
esac
