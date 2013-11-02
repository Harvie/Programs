#!/bin/bash
#Experimental BASH script to compile DHCP options

dec_to_hex() {
	echo 'obase=16; '"$1" | bc
}

dec_to_hex_right() {
	echo -n $(while true; do echo -n 0; done | head -c "$2")$(dec_to_hex "$1") | tail -c "$2"
}

escape() {
	while read -n 2 i; do
		[ -n "$i" ] && echo -n '\x'"$i";
	done
}

dnsmasq() {
	sed -e 's/^\\x//g' | sed -e 's/\\x/:/g'
}

dhcp_option() {
	separator='\x'
	option_id="$1"
	option_data="$2"

	len=$(echo -n "$option_data" | wc -c)
	dec_to_hex_right $option_id 2 | escape
	dec_to_hex_right $len 2 | escape
	printf "$option_data" | xxd -ps -c 256 | escape
}

pd() {
	option_id=25
	iaid='\x00\x00\x00\x00'
	t1='\x00\x00\x01\x2C'
	t2='\x00\x00\x01\x2C'
	ia_pd_opts=''
	dhcp_option $option_id "$iaid$t1$t2$id_pd_opts"
}

pd | dnsmasq
echo
