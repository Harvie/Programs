#!/bin/sh
#
#Serial line tester v0.3
#(c) Tomas Mudrunka 2022-2024
#
#Tested in: busybox ash, bash

function serial_loopback_test() {

unset port_tx
unset port_rx
trylimit=0
seconds=0
baudrate=115200
workaround_rs485=false

while getopts ":t:r:b:l:s:w" OPT; do
	#echo "$OPT ==> $OPTARG";
	case $OPT in
		t) port_tx=$OPTARG;;
		r) port_rx=$OPTARG;;
		b) baudrate=$OPTARG;;
		l) trylimit=$OPTARG;;
		s) seconds=$OPTARG;;
		w) workaround_rs485=true;;

		'?') echo "Usage: $0 -t /dev/ttyTX [-r /dev/ttyRX] [-b 9600] [-l 10] [-s 10] [-w]"	>&2; return 250;;
		':') echo "Missing option argument for -$OPTARG"					>&2; return 251;;
		*  ) echo "Unimplemented option: -$OPT"							>&2; return 252;;
	esac
done

[ -z "$port_tx" ] && port_tx="$port_rx"
[ -z "$port_rx" ] && port_rx="$port_tx"
[ -z "$port_rx" ] && { echo Port not specified; return 253; }


testdata="${RANDOM}d82b2ae45432e7c80699852ab557b279c42180a379711aae85487bda0cc58602f65c0ab5af0d510d6ac1606c52f887f3332052f67c45212dbbf2730${RANDOM}"

echo "Running $baudrate baud loopback test on $port_tx -> $port_rx"
echo "Limiting to $trylimit iterations and $seconds seconds (0 = no limit)"

stty -F "$port_tx" raw -echo "$baudrate"
stty -F "$port_rx" raw -echo "$baudrate"

exec 5>"$port_tx"
exec 6<"$port_rx"

$workaround_rs485 && {
	echo "Deasserting RTS on buggy RS485 drivers by writing to RX port"
	echo>"$port_rx"
	read -rst 1 -u 6
}

errcnt=0
trycnt=0
datestart="$(date +%s)"
dateend="$(($datestart+$seconds))"
while true; do
	echo "$testdata" >&5
	unset line
	read -rst 1 -u 6 line
	#echo "LINE: $line"
	[ "$line" != "$testdata" ] && errcnt=$((errcnt+1)) &&
		echo -e "$(date +'%D %T')\t$errcnt\tError receiving data $port_tx -> $port_rx"
	trycnt=$((trycnt+1))
	#echo "try $trycnt err $errcnt lim $trylimit"
	[ $trylimit -gt 0 -a $trycnt -gt $trylimit ] && break
	[ $seconds -gt 0 ] && [ $(date +%s) -gt $dateend ] && break
done

exec 5>&-
exec 6<&-

echo TRANSMITTED: $trycnt, ERRORS: $errcnt, SECONDS: $(($(date +%s)-$datestart))
[ "$errcnt" = "0" -a "$trycnt" -gt 0 ] && return 0
return 255

}

serial_loopback_test "$@"
