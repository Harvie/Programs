#!/bin/bash
#HTTPd.bash (Harvie 2oo9)
#WebServer+Small-INSECURE+Emergency+Filelisting+Mime+Unicode-BASH
#TODO: Security, Dynamic pages in BASH/PHP, Speed
#dependencies: GNU NetCat, Links, sed

listen_port=8080;
web_root='/home/harvie';
nc='nc'; links='links'; #binaries

if [ "$1" == '--serve' ]; then
	urldecode() { echo -e $( echo "$@" | sed -e 's/+/ /g;s/%/\\x/g' ); }

	get="$( head -n 1 | cut -d ' ' -f 2 )";
	get="$web_root$(urldecode $get)";

	if [ -d "$get" ]; then content_type='text/html;charset=utf-8';
	else content_type=$(file -i "$get" | cut -d ' ' -f 2-); fi;

	echo -ne \
'HTTP/1.1 200 OK
Server: HTTPd.bash
Content-Type: '"$content_type"'
Connection: close\n\n';

	if [ -d "$get" ]; then "$links" -source "$get"; else cat "$get"; fi;
	exit;
fi;

listen() { "$nc" -l -p "$listen_port" -e "kill -s USR1 '$$' & '$0' --serve"; }

trap 'listen &' SIGUSR1;
trap "echo -e '\nHTTPd.bash stoped'; exit" SIGINT;
kill -s USR1 "$$";
echo -e "HTTPd.bash started\nHarvie 2oo9\nport: $listen_port\nroot: $web_root"
while true; do sleep 0.1; done;
