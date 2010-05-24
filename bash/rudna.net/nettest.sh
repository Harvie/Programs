#!/bin/bash
#Poustime pod r00tem!
testbtotal=268943360; #wc -c test.bin

list_victims() {
	#tahle funkce zjisti a vypise IP/hostname testovanych boxu - na radku jeden
	echo fsdfdf
	echo strep #dostupna + test.bin k dispozici
	echo 127.0.0.1 #dostupna
	echo 192.168.2.1 #nedostupna sit
	echo lollol #neexistujici dns
}

ping_perc() {
	perc=0;
	for i in $(seq 0 99); do
		ping -c 1 -w 0.7 "$1" &> /dev/null;
		ret="$?";
		if [[ "$ret" == 0 ]]; then
			perc=$[$perc+1];
		fi;
		if [[ "$ret" == 2 ]]; then
			echo 0; return 0;
		fi;
	done;
	echo "$perc";
	return "$perc";
}

for victim in $(list_victims); do
	#victim
		echo -n "$victim - ";
	#ping
		echo -n "ping responses: ";
		pings=$(ping_perc "$victim");
		echo -n "$pings%, ";
	#test.bin
		echo -n "test.bin: "
		bytes=$(curl "http://$victim/test.bin" --stderr /dev/null|wc -c);
		downloaded=$[$bytes*100/$testbtotal];
		echo -n "$downloaded% downloaded, ";
	#endvictim
		echo -n "; ";
done;

echo;

#rozsahu, ty pingnout -c 100, potom z nich stahnout test.bin, do nagiosu potom poslat zpravu spoj $ESSID ma $ODEZVA odezvu, $LOSS, rychlost $WGET_AVG 
