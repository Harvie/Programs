#!/bin/bash
#BANNER#######################################
echo -n '
 ____  ____  __ _  _  _   __   __ _
(__  )(  __)(  ( \( \/ ) / _\ (  ( \
 / _/  ) _) /    // \/ \/    \/    /
(____)(____)\_)__)\_)(_/\_/\_/\_)__) v1.5

  Zenity based frontend for pacman
  <~Harvie (http://blog.harvie.cz)
  2oo9

';

#HELP/FEATURES################################

if [ "$1" == '--help' ]; then
cat << END
HELP/FEATURES:
- zenman --help prints this help
- when zenman started, kills last zenman runned by this user
	- you can kill zenman by zenman --stop (click on tray icon or wait after this)
	- last pid is stored in ~/.config/zenman/zenman.pid
- shows splashscreen (can be disabled...)
- shows tray icon for each state (on mouse over shows state)
	- system is up to date
	- refreshing pacman database
	- updates available
- shows notification when updates available and listing them
- clicking on icon will begin upgrade in new xterm
	- this is disabled only when refreshing
	- if pacman db is locked it will be automaticaly unlocked
		- only if there is any pacman related process running
		- if there is some pacman related operation, error message appears
- auto refreshing of pacman db is disabled (somebody will want to use cron)
- all settings are hardcoded to zenman (/usr/bin/zenman)
  - all settings can be overrided
		- system wide: /etc/zenman.conf
		- single user only: ~/.config/zenman/zenman.conf
		- you can also write your own translation to this files

TODO:
( Don't shy and get involved! ;o)
- Bugtracking/Bugfixing
- Port to another package managers
  - apt-zen (apt-get)
  - zenmerge (emerge)
  - ipkg-zen (ipkg)
  - etc...
- Localization to your native language
END
exit;
fi;

#SETTINGS#####################################
    zenman_caption='ZenMan';
   zenman_interval=$[10*60];
     zenman_splash=true;
zenman_autorefresh=false;
  zenman_configdir=~/.config/zenman;
        zenman_pid="$zenman_configdir/zenman.pid";
   zenman_true_ico='/usr/share/icons/gnome/scalable/status/software-update-urgent.svg'; #software-update-available.svg
  zenman_false_ico='/usr/share/icons/gnome/scalable/emotes/face-cool.svg';
zenman_refresh_ico='/usr/share/icons/gnome/scalable/actions/view-refresh.svg';

              gksu='gksu'; #gksu/gksudo/...
            pacman='/usr/bin/pacman'; #want to use pacman-color, yaourt or powerpill instead?
             xterm='xterm'; #xterm/...
    pacman_refresh='-Syuw'; # -Sy ?
    pacman_upgrade='-Syu'; # -Syuf ?
      pacman_check='-Qu';
       pacman_lock='/var/lib/pacman/db.lck'; #file to delete, when pacman locked (only if not running)

       zenman_true='New package(s) found! ;-)\n\nPacman output follows:\n';
      zenman_false='No packages to update. ;-(';
 zenman_refreshing='Refreshing pacman database. ;-) Please wait...';
    zenman_upgrade="[*] ZenMan will now execute full system upgrade! ;-)\n'$pacman' '$pacman_upgrade'\n\n";
      zenman_close='[*] Press [ENTER] to close window...';
     zenman_config='[*] Loading configuration from: ';
       zenman_done='[DONE]';

     zenman_locked="ZenMan can't execute system upgrade! ;-(
Please stop all pacman related applications and try again.";

      zenman_start="ZenMan started! ;-)
by: Harvie 2oo9
For more informations please visit:
http://blog.harvie.cz/";

mkdir -p "$zenman_configdir"
touch "$zenman_configdir/zenman.conf"
#read configuration files (can contain localization also)...
for i in\
	'/etc/zenman.conf'\
	"$zenman_configdir/zenman.conf"\
; do
	echo "$zenman_config" "$i";
	source "$i" >/dev/null 2>&1;
done;

#fallback icons
   if [ ! -e "$zenman_true_ico" ]; then zenman_true_ico='warning'; fi;
  if [ ! -e "$zenman_false_ico" ]; then zenman_false_ico='info'; fi;
if [ ! -e "$zenman_refresh_ico" ]; then zenman_false_ico='question'; fi;


#CODE#########################################
#pid
if [ -e "$zenman_pid" ]; then
	kill $(cat "$zenman_pid") >/dev/null 2>&1;
	kill -KILL $(cat "$zenman_pid") >/dev/null 2>&1;
fi;
if [ "$1" == '--stop' ]; then
	echo '[!] exit';
	exit;
fi;
echo "$$" > "$zenman_pid";

#splash
echo '[i]' "$0" '--stop' to kill zenman
echo '[i]' "$0" '--help' to show help
if "$zenman_splash"; then
	notify-send -i info --expire-time=5000 "$zenman_caption" "$zenman_start";
fi;

#unlocker
zenman_force_unlock() {
	if [ -e "$pacman_lock" ]; then #check if db locked...
		ps -eo comm | grep -i '^\(pacman\|yaourt\|powerpill\|gtkpacman\|pacupdate\|kpacupdate\|guzuta\|YAPG\|Shaman\)' >/dev/null; #check if pacman is running...
  	if [ $? != 0 ]; then
			"$gksu" "rm -rf '$pacman_lock'";
			if [ $? != 0 ]; then
				return 1; #gksu failed ;-(
			else
				return 0; #unlocked!!!
			fi;
		else
			return 1; #pacman is running ;-(
		fi;
	else
		return 0; #no locked!!!
	fi;
}

while true; do
	#uncomment next line if not running -Syuw from cron (and you better set higher zenman_interval)
	if "$zenman_autorefresh"; then
		echo -ne "$zenman_refreshing ";
		zenity --notification --text "$zenman_refreshing" --window-icon="$zenman_refresh_ico" &
			zenman_force_unlock;
			"$gksu" "'$pacman' $pacman_refresh --noconfirm" >/dev/null 2>&1
		kill $! >/dev/null 2>&1;
		kill -KILL $! >/dev/null 2>&1;
		echo "$zenman_done";
	fi;
	#check for new packages
	pacman_new_packages=$("$pacman" $pacman_check --noconfirm 2>&1);
	if [ $? == 0 ]; then
		#new packages found
		zenman_block='read';
		notify-send -i info "$zenman_caption" "$(echo -e "$zenman_true" "$pacman_new_packages")"
		zenity --notification --timeout="$zenman_interval" --text "$(echo -e "$zenman_true" "$pacman_new_packages")" --window-icon="$zenman_true_ico";
	else
		#no packages to update
		zenman_block='';
		zenity --notification --timeout="$zenman_interval" --text "$zenman_false" --window-icon="$zenman_false_ico";
	fi;

	#run full system upgrade if notification icon clicked
	if [ $? == 0 ]; then
		#try to unlock pacman db if locked and pacman is not running
		zenman_force_unlock;
		if [ $? == 0 ]; then
			#update if unlocked
			"$xterm" \
				-T "$zenman_caption"\
				-e "echo -ne '${zenman_upgrade}'; '$gksu' '\"${pacman}\" $pacman_upgrade'; echo -e '\n${zenman_close}'; $zenman_block";
			zenman_force_unlock;
		else
			#show error if locked
			notify-send -i error "$zenman_caption" "$zenman_locked";
		fi;
	fi;
done;
