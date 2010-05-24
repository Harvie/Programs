#!/bin/bash
#Harvie's MAAADWiFi module instalation script
#2oo8
#No freakin' rightz reserved!

cd /usr/src
rm -rf madwifi-source*

apt-get build-dep madwifi-source

echo Downloading+Building madwifi-source. plz w8...;
apt-get source -b madwifi-source
if [[ $? > 0 ]]; then
	echo 'apt-get source ERROR... Sorry dude ;(';
	return 1;
fi;

echo Instaling madwifi module. plz w8...;
dpkg -i madwifi-source*.deb
if [[ $? > 0 ]]; then
	echo 'dpkg -i ERROR... Sorry dude ;(';
	return 1;
fi;

echo "Madwifi gracefully orgasmed. You're da best *NIXadmin on the world.";
echo "Chuck Norris is proud of you...";
echo;
