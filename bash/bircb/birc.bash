#!/bin/bash
 # BIRC(B) 1.3b - BASH IRC (Bot)
 # Simple IRC Bot/Client with SSL and SOCKS support written in BASH
 # <~~Harvie 2oo7/8

#WTF?
 # You can use this as bot or client if anything else can't be installed.
 # In fact, BIRC is just a netcat frontend to act as IRC client.
 # If you don't have netcat, you can use other similar software.
 
#Setting NETCAT
 # If you want use insecure (non-ssl) connection, there are suitable configurations:
 #  format: 	# package		-> NETCAT="value"
	# netcat			-> NETCAT="nc"
	# telnet			-> NETCAT="telnet"
	# socat				-> NETCAT="birc_socat"
	# socat (for IPv6)		-> NETCAT="birc_socat_6"
	
	# tail
	# bash>=3 with /dev/tcp supp.	-> NETCTA="birc_bash_netcat"
		# With this, you don't need any external binary
		# Your BASH have to be compiled with /dev/tcp/ support (problem on Debian,...)
		# I never tested this wrapper ;(

#Setting NETCAT for SSL
 # Remember that default settings is to accept any cert, which means it is less secure...
 # If you need to be in safe againtst MITM attacks, read something and edit wrappers.
 # Remember that IRC server is sending your messages to other clients (that should not use SSL)
 # You can determine if any user is connected with SSL by /whois nickname.
 # 1.) Change BIRCPORT to port used by server for SSL
 # 2.) Select one of these solutions (depends on software, you have installed):
 #  format:	# package		-> NETCAT="value"
	#  socat (for SSL)		-> NETCAT="birc_socat_ssl"
	#  stunnel 			-> NETCAT="birc_stunnel"
	#  telnet-ssl 			-> NETCAT="birc_telnet_ssl"

#Setting NETCAT for TOR network (or another SOCKS server)
 # My SOCKS wrapper is set to use SOCKS4A server on 127.0.0.1:9050 which is used by TOR
 # If you want to use another SOCKS server, you can simply edit the wrapper.
 # Read "Setting NETCAT for SSL" for more info about SSL.
 # After connecting check /whois yournickname to get your new IP adress...
 # This will need socat installed and TOR client running...
 #  format:	# package		-> NETCAT="value"
	#  socat + tor (for TOR)	-> NETCAT="birc_socat_tor"
	#  socat + tor (for TOR&SSL)	-> NETCAT="birc_socat_tor_ssl"
		# TOR+SSL = F*cking great privacy!

#Setting NETCAT for different encoding
 # This example shows you, how you can use uconv to convert encoding if server/channel
 # requires some specific setting.
  # NETCAT="nc"
  # BIRCCODE="UTF-8"
  # NETCAT="uconv -t $BIRCCODE -s -c -i -b 1 | $NETCAT | uconv -f $BIRCCODE -s -c -i -b 1"

#Setting as bot
 # You have to add more functions into the birc_parse().
 # Maybe, you want to make birc smaller by stripping this comments.
 # I like to compress/obfuscate the code by gzexe.

#Setting as client
 # You can add some startup menu, or incoming PRIVMSG parser.
 # It's good idea to make some kind of multichannel support.

#HELP/INFO
 # $0 -h == help
 # CTCP examples:
 	# ^AACTION ROX!!!^A == "/ME ROX!!!"
 	# ^APING Suxor^A == "/PING Suxor"
 	# etc...
 # Licence policy: eNarchy ultrafree opensource copylefted public domain ;D
 # Other questions -> !!!-UTFS-FM-OMG-!!!
 # Deps: 
	# BASH>=3
	# tail
	# one of these: (netcat|telnet(-ssl)?|socket|stunnel|socat) - set NETCAT in settings

#################################################################################################

#DEFAULT_SETTINGS (Can be overriden by command-line arguments)
 #Host and port:
 BIRCHOST=irc.lukysoft.sk
 BIRCPORT=6667
 #Nic to use:
 BIRCNICK=bircbot
 #Channel to join:
 BIRCCHAN=\#skola
 #Wait N seconds before first try to join:
 BIRCWAIT=1
 #Try to rejoin after N seconds since connected:
 BIRCJOIW=10
 #Clean socket file every N seconds:
 BIRCLEAN=120
 #Where to save socket file?
 BIRCSDIR=~/.birc
 BIRCSOCK="$BIRCSDIR"/sock
 #NetCat binary (see comments ^^^):
 NETCAT="birc_bash_netcat"
 #NET/IO Interval (tail -s N):
 TAILSLEEP=0.3
  
#################################################################################################

#BIRC_NETCAT_WRAPPERS (RTFM)
 birc_socat() { socat STDIO TCP4:"$1":"$2"; }
 birc_socat_ssl() { socat STDIO OPENSSL:"$1":"$2",verify=0; }
 birc_socat_6() { socat STDIO TCP6:"$1":"$2"; }
 birc_stunnel() { stunnel -c -r "$1":"$2"; }
 birc_telnet_ssl() { telnet-ssl -z ssl "$1" "$2"; }
 birc_bash_netcat() {
 	exec 5<>"/dev/tcp/$1/$2";
 	cat <&5 &
	cat >&5;
 }

 #SOCKS Server settings:
  SOCKSSERV=127.0.0.1
  SOCKSPORT=9050

 birc_socat_tor() { socat STDIO SOCKS4A:"$SOCKSSERV":"$1":"$2",socksport="$SOCKSPORT"; }
 birc_socat_tor_ssl() {
 	BIRCTUNPORT=$[ 1025+($RANDOM%9999) ]
 	echo [i] Starting SOCKS4A tunnel to "$1":"$2" on random port = "$BIRCTUNPORT";
 	socat TCP4-LISTEN:"$BIRCTUNPORT",fork SOCKS4A:"$SOCKSSERV":"$1":"$2",socksport="$SOCKSPORT" &
 	socat STDIO OPENSSL:127.0.0.1:"$BIRCTUNPORT",verify=0;
 }

#################################################################################################

#BIRC_FUNCTIONS (BIRC Library)
 birc_help() {
	# BIRC Help - prints help and exit
	echo "BIRC - BASH IRC (lib,client,bot) - Harvie 2oo7";
	echo -e "\tUsage:";
	echo -ne "\t$0 ";
	echo "[server [port [nick [channel [ sockfile [ netcatbin]]]]]]";
	echo -e "\tDefault: $BIRCHOST $BIRCPORT $BIRCNICK $BIRCCHAN $BIRCSOCK $NETCAT";
	echo;
	exit;
 }

 birc_parse() {
	# BIRC Parse (data, socket)
	# You can handle each incoming line ($1) here

	#PRINT
	echo "$1";

	#PING/PONG
    if [[ "$1" =~ ^PING' '*:(.*) ]]; then
    	echo "PONG :${BASH_REMATCH[1]}"
			echo "PONG :${BASH_REMATCH[1]}" >> "$2"
    fi;

	# -> MORE BOT-FUNCTIONS HERE <-
	# BASH is cool, you can just call another "module" script with bot functions from here.
 }


 birc_connect() {
	# IRC Connect (socket, host, port)
	# Create new socket fifos...
	rm -f "$1"; touch "$1";
	rm -f "$1"r; touch "$1"r;

	# Open connection and pipes on background
	birc_startnc() {
		tail -f --retry -s "$TAILSLEEP" "$1" 2> /dev/null | "$NETCAT" "$2" "$3" >> "$1"r;
		# Close birc after connection closed
		kill -2 $$; sleep 1; kill -9 $$;
	}
	birc_startnc "$1" "$2" "$3" &

	# Recieve and process incoming commands
	tail -f --retry -s "$TAILSLEEP" "$1"r 2> /dev/null | while read BIRCLINE; do
		birc_parse "$BIRCLINE" "$1"
	done &
 }

 birc_login() {
	# IRC Login (socket, nick)
	echo NICK "$2" >> "$1"
	echo USER "$2 $2 $2 :$2" >> "$1"
	echo >> "$1"
 }

 birc_join() {
	# IRC Join (socket, channel)
	echo JOIN "$2" >> "$1"
 }

 birc_delayed_join() {
	# IRC Join with delay on BG (socket, channel, delay (secs))
	sleep "$3" && birc_join "$1" "$2" &
 }

 birc_say() {
	# IRC Send (socket, data[, receiver])
	# -> MORE USER-FUNCTIONS HERE <-

	if [[ "$2" =~ ^/(.*) ]]; then
		#Server command
  	echo "${BASH_REMATCH[1]}" >> "$1"
  else
		#Common message
		echo PRIVMSG "$3" :"$2" >> "$1"
	fi;
 }

 birc_cleanup() {
	# BIRC Cleanup (socket)
	# Cleanup mess leaved in system before BIRC exit
	kill -s SIGINT  $(jobs -p); > /dev/null 2>&1
	kill -s SIGKILL $(jobs -p); > /dev/null 2>&1
	echo \[!\] All background jobs stoped!
	rm -f "$1"; > /dev/null 2>&1
        rm -f "$1"r; > /dev/null 2>&1
	echo \[!\] All temp files removed!
        echo \[X\] Quiting BIRC...
	exit;
 }

 birc_autocleand() {
	# BIRC Autoclean Daemon (socket, delay)
	# starts on background, clean socket each $2 seconds
	while true; do
		sleep "$2";
		echo -n > "$1" > /dev/null 2>&1;
		echo -n > "$1"r > /dev/null 2>&1;
	done &
 }

#################################################################################################

#MAIN_C0DE (BIRC-Lib example)
 #ARGUMENTS
 if [ "$1" == "-h" ]; then birc_help; fi;
 if [ -n "$1" ]; then BIRCHOST="$1"; fi;
 if [ -n "$2" ]; then BIRCPORT="$2"; fi;
 if [ -n "$3" ]; then BIRCNICK="$3"; fi;
 if [ -n "$4" ]; then BIRCCHAN="$4"; fi;
 if [ -n "$5" ]; then BIRCSOCK="$5"; fi;
 if [ -n "$6" ]; then NETCAT="$6"; fi;

 echo \[*\] Starting BASH IRC Client\\Bot
	trap "echo; echo \[X\] Caught SIGINT - terminating...; birc_cleanup \"$BIRCSOCK\"" SIGINT;
	mkdir -p "$BIRCSDIR";
 echo \[I\] Written by \<-Harvie 2oo7;
 echo;

 echo \[i\] Using socket wrapper "$NETCAT";
 echo \[i\] Using socket Files/FIFOs "$BIRCSOCK"\(r\);
 echo \[i\] Using socket interval "$TAILSLEEP" seconds between I/O;
	birc_connect "$BIRCSOCK" "$BIRCHOST" "$BIRCPORT";
	birc_autocleand "$BIRCSOCK" "$BIRCLEAN";
		sleep 1;

 echo \[i\] "$USER"@$(hostname) -\> "$BIRCNICK@$BIRCCHAN@$BIRCHOST:$BIRCPORT";
	birc_login "$BIRCSOCK" "$BIRCNICK";
                sleep "$BIRCWAIT";

 echo \[i\] Joining channel "$BIRCCHAN";
	birc_join "$BIRCSOCK" "$BIRCCHAN";
	birc_delayed_join "$BIRCSOCK" "$BIRCCHAN" "$BIRCJOIW";

 echo \[\>\] Now waiting for your messages on STDIN;
	while true; do
		read BIRCSEND;
		birc_say "$BIRCSOCK" "$BIRCSEND" "$BIRCCHAN";
	done;

 birc_cleanup "$BIRCSOCK";
 exit;
