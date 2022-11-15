#!/bin/bash
#(c) Tomas Mudrunka 2022

[ -z "$1" ] && {
	echo "Usage: $0 /dev/ttyTX [/dev/ttyRX]"
	exit 1
}

port_tx="${1:-/dev/ttyUSB0}"
port_rx="${2:-$port_tx}"

testdata="${RANDOM}d82b2ae45432e7c80699852ab557b279c42180a379711aae85487bda0cc58602f65c0ab5af0d510d6ac1606c52f887f3332052f67c45212dbbf2730${RANDOM}"

echo "Running loopback test on $port_tx -> $port_rx"

stty -F "$port_tx" raw -echo
stty -F "$port_rx" raw -echo

exec 5>"$port_tx"
exec 6<"$port_rx"

errcnt=0
while true; do
	echo "$testdata" >&5
	unset line
	read -rst 1 -u 6 line
	#echo "LINE: $line"
	[ "$line" != "$testdata" ] && {
		((errcnt=errcnt+1))
		echo -e "$(date +'%D %T')\t$errcnt\tError receiving data $port_tx -> $port_rx"
	}
done

exec 5>&-
exec 6<&-
