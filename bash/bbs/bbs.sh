#!/bin/bash
#Settings
bbs_dir=$(dirname "$0");
bbs_title="Harvie's BBS";
#Misc
fail_delay="5";

#Directories
config_dir="./config";
data_dir="./data";
utils_dir="./utils";
#Data directories
chatrooms_dir="$data_dir/chatrooms";
boards_dir="$data_dir/boards";
archive_dir="$data_dir/archive";
#Files
welcome_file="$config_dir/welcome";
about_file="$config_dir/about";
farewell_file="$config_dir/farewell";
users_file="$config_dir/users";
log_file="$config_dir/security.log";

#Init...
cd "$bbs_dir";
tmp="/tmp/bbstmp-$$";
dialog=$(which dialog);
export DIALOGOPTS="--no-shadow --trim --title \"$bbs_title\" --backtitle \"$bbs_title\"";
w(){ echo $(tput cols); }						#dialog width
h(){ echo $(($(tput lines)-0)); }		#dialog height (LINES-5 to display backtitle)
m(){ echo $(($(tput lines)-12)); }		#dialog menu size
bbs_ssh_ip=$(echo $SSH_CLIENT | cut -d' ' -f1); #Works only with OpenSSH
alias ls=ls; #Get rid of that 'user friendly' crap ;)

#Functions...
bbs_welcome_banner() {
  cat "$welcome_file" > "$tmp";
  echo "$SSH_CLIENT" >>"$tmp"; #Works only with OpenSSH
	$dialog --title "Welcome to $bbs_title" --exit-label "Enter" --textbox "$tmp" $(h) $(w);
	if [ "$?" != 0 ]; then bbs_exit; fi;
}

bbs_about() {
	$dialog --title "About $bbs_title" --exit-label "Agree" --textbox "$about_file" $(h) $(w);
}

bbs_exit() {
	rm -f "$tmp";
	clear;
	cat "$farewell_file";
	exit;
}

bbs_check_login() {
  cat "$users_file" | grep "^[^# ]" | while read i; do
  	login=$(echo "$i" | cut -d: -f1);
  	passw=$(echo "$i" | cut -d: -f2);
  	group=$(echo "$i" | cut -d: -f3);
  	if [[ "$1" == "$login" && "$2" == "$passw" ]]; then
  		echo "$group";
			echo "($(date +%T)) $bbs_ssh_ip: Login to user $1 successfull" >>"$log_file";
  	fi;
  done;
}

bbs_preview() {
	if [[ -d "$1" ]];
		then echo '[DIR]' #$(ls -A "$1"); #<- uncomment rest of the line to have directory preview
		else echo #$(head -n1 "$1"); #<- uncomment rest of the line to have file preview
	fi;
}

bbs_browse_dir() { #brand new dialog filelisting - supposed to be secure to directory traversal!
	while true; do

		#call dialog with output of ls...
  	ls -A1 --group-directories-first "$1" | while read i; do
	  	echo -n "$i"; echo -ne "\x00";
  		echo -n $(bbs_preview "$1/$i");  echo -ne "\x00"
  	done | xargs -0 $dialog --menu -- "Select the board" $(h) $(w) $(m) 2>"$tmp";

  	if [ "$?" != 0 ]; then
			return 1;
			#$dialog --pause "HOOOPS! Sorry. None or empty directory selected..." $(h) $(w) "$fail_delay";
		else
      selected=$( echo -n "$1/"; cat "$tmp" );
			if [[ -d "$selected" ]];
				then
					bbs_browse_dir "$selected"; return "$?";
				else
					echo "$selected" >"$tmp"; return 0;
			fi;
		fi;

	done;
}

bbs_browse_dir() { #brand new dialog filelisting - supposed to be secure to directory traversal!
	while true; do

  	"$utils_dir/fselect" "$1" "$tmp"

  	if [ "$?" != 7 ]; then
			return 1;
			#$dialog --pause "HOOOPS! Sorry. None or empty directory selected..." $(h) $(w) "$fail_delay";
		else
			#$dialog --pause "HOOOPS! Sorry. None or empty directory selected..." $(h) $(w) "$fail_delay";
      #selected=$( echo -n "$1/"; cat "$tmp" );
			#echo "$selected" >"$tmp";
			return 0;
		fi;

	done;
}

#bbs_file_select() { #deprecated
#  ls -A1 $@ | while read i; do
#        echo -n "$i"; echo -ne "\x00";
#        echo -n $(bbs_preview "$1/$i");  echo -ne "\x00"
#  done | xargs -0 $dialog --menu -- "Select the file" $(h) $(w) $(m);
#  return "$?";
#}

bbs_file_edit() {
	#This is supposed to be a secure way to edit the file...
	nano -DR "$1";
}

#Chat##################

bbs_chat_send() {
  $dialog --inputbox "Enter message" $(h) $(w) 2>"$tmp"
  if [ "$?" != 0 ]; then return; fi;
  message=$(cat "$tmp");
  echo "($(date +%T))" "$login: $message" >>"$1"
}

bbs_chat_join() {
	while true; do
  	$dialog --title "Chatroom $1 (esc to leave)" --exit-label "Write message" --tailbox "$1" $(h) $(w);
	if [ "$?" != 0 ]; then return; fi;
	bbs_chat_send "$1";
	done;
}

bbs_chat() {
  bbs_browse_dir "$chatrooms_dir";
  if [ "$?" != 0 ]; then return; fi;
  chatroom=$(cat "$tmp");
  bbs_chat_join "$chatroom";
}



#Mail####################

bbs_mail() {
	$dialog --pause "HOOOPS! Sorry. Mail not yet ;o)" $(h) $(w) "$fail_delay";
}

#Archive#################

bbs_board_read() {
	#echo BBS_BOARD_READ "$1" | less;
	#less "$1";
	$dialog --textbox "$1" $(h) $(w);
}

bbs_archive() {
	while true; do
		bbs_browse_dir "$archive_dir";
		if [ "$?" != 0 ];
			then break;
			else bbs_board_read $(cat "$tmp");
		fi;
	done;
}

#Forum###################

bbs_board() {
	while true; do
		bbs_browse_dir "$boards_dir";
		if [ "$?" != 0 ];
			then break;
			else bbs_board_read $(cat "$tmp");
		fi;
	done;
}


#System menu#############

bbs_admin_console() {
	#While this BBS is not complete, i will give the shell to the admin group to fix-up everything ;o)
	#bash can be replaced by chrooted SASH (ex.) to improve security...
	$dialog --insecure --passwordbox "Enter your password" $(h) $(w) 2>"$tmp";
	if [ "$?" != 0 ]; then return; fi;
  repassword=$(cat "$tmp");
 	if [[ "$group" == "admin" && "$repassword" == "$password" ]]; then
		clear;
		echo -e "This is management console. Press ctrl+d or type exit to leave...\n";
	  bash;
	  clear;
	else
		$dialog --pause "Authentification failed. Check your password and/or membership in admin group." $(h) $(w) "$fail_delay";
	fi;
}

bbs_logs() {
	if [ "$group" == "guest" ]; then
		$dialog --pause "Users from guest group can't browse logs!" $(h) $(w) "$fail_delay";
		return;
	fi;
	$dialog --textbox "$log_file" $(h) $(w);
}

bbs_user_info() {
	echo "LOGIN: $login" >"$tmp";
	echo "GROUP: $group" >>"$tmp";
	echo >>"$tmp";
  echo "IP (if any): $SSH_CLIENT" >>"$tmp"; #Works only with OpenSSH
  echo "CONNECTION (if any): $SSH_CONNECTION" >>"$tmp"; #Works only with OpenSSH
	$dialog --title "Userinfo for $login" --textbox "$tmp" $(h) $(w);
}

bbs_user_list() {
  cat "$users_file" | grep "^[^# ]" | while read i; do
  	login=$(echo "$i" | cut -d: -f1);
  	group=$(echo "$i" | cut -d: -f3);
  	echo -e "$group\t\t$login";
  done | sort -u >"$tmp";
	$dialog --title "Userlist for $bbs_title" --textbox "$tmp" $(h) $(w);  
}

bbs_register() { ########################################################<-incompleteeeeeeeee
	echo -n "$SSH_CLIENT:$ " 2>"$tmp";
	$dialog --inputbox "Send message to admins\n\
		eg.:\nREGISTRATION: yournick:yourpasswd CONTACT:your@adress.net Few words about you here...\n\
		or\nSome other message..." $(h) $(w) 2>>"$tmp";
	if [ "$?" != 0 ]; then return; fi;
}

bbs_contact_admins() { ########################################################<-incompleteeeeeeeee
	echo -n "$SSH_CLIENT:$ " 2>"$tmp";
	$dialog --inputbox "Send message to admins\n\
		eg.:\nREGISTRATION: yournick:yourpasswd CONTACT:your@adress.net Few words about you here...\n\
		or\nSome other message..." $(h) $(w) 2>>"$tmp";
	if [ "$?" != 0 ]; then return; fi;
}

bbs_system_menu() {
	while true; do
		$dialog --menu "System Menu" $(h) $(w) $(m)\
			userinfo	"Info about you"\
			userlist	"Info about other users"\
			logs			"View logs"\
			register	"Submit request for new account"\
			contact		"Contact administrators"\
			admin			"Enter admin management console..."\
			back			"Go back to the main menu..."\
		2>"$tmp"
		if [ "$?" != 0 ]; then return; fi;

		choice=$(cat "$tmp")
		case "$choice" in
	  	"userinfo")	bbs_user_info;;
	  	"userlist")	bbs_user_list;;
	  	"logs")			bbs_logs;;
	  	"register")	bbs_register;;
	  	"contact")	bbs_contact_admins;;
	  	"admin") 		bbs_admin_console;;
	  	"back") 	return;;
	  	*) 				echo bad choice... keep trying.;;
		esac
	done;
}

#CODE###########################################################################
#Welcome
bbs_welcome_banner;

#Login
while true; do
	$dialog --inputbox "Enter your nickname" $(h) $(w) 2>"$tmp";
		if [ "$?" != 0 ]; then bbs_exit; fi;
		login=$(cat "$tmp");
	$dialog --insecure --passwordbox "Enter your password" $(h) $(w) 2>"$tmp";
		if [ "$?" != 0 ]; then bbs_exit; fi;
		password=$(cat "$tmp");

  group=$(bbs_check_login "$login" "$password");
  case "$group" in
  	"user")		break;;
  	"admin")	break;;
  	"guest")	break;;
  esac;
  $dialog --pause "Login failed! Press enter to continue..." $(h) $(w) "$fail_delay";
	echo "($(date +%T)) $bbs_ssh_ip: Login to user $login failed" >>"$log_file";
done;

if [[ "$group" == "guest" && -n "$bbs_ssh_ip" ]]; then
	login="[$bbs_ssh_ip]$login";
fi;

	#Login debug ;)
  #echo Loged as "$login:$group":$(mkpasswd "$password"); bbs_exit;

while true; do
	$dialog --no-cancel --menu "Main Menu" $(h) $(w) $(m)\
		chat		"Browse online chatrooms..."\
		boards	"Browse boards..."\
		archive	"Browse archive..."\
		mail		"Send/receive personal messages..."\
		system	"Enter system menu..."\
		about		"About this BBS..."\
		exit		"Logout"\
	2>"$tmp"

	choice=$(cat "$tmp")
	case "$choice" in
	  "chat") 		bbs_chat;;
	  "boards")		bbs_board;;
	  "archive")	bbs_archive;;
	  "mail")			bbs_mail;;
	  "system")		bbs_system_menu;;
	  "about")		bbs_about;;
	  "exit") 		bbs_exit;;
	  *) 					$dialog --pause "HOOOPS! Sorry. $choice not yet ;o)" $(h) $(w) "$fail_delay";;
	esac
done;

bbs_exit;
