#!/bin/bash

clear
cat etc/win

echo You won $1 level!!!
echo -n Enter your nick \(or type exit\): ""
read nick

if [ $nick = "exit" ]; then
	echo You weren\'t written to list of winners... ;
	exit;
fi;

echo $1:$nick >> var/log/winners
echo Written to list of winners as $1:$nick!
exit
