#!/bin/bash
#Experimental BASH script to compile DHCP options

dec_to_hex() {
	echo 'obase=16; '"$1" | bc
}

dec_to_hex_right() {
	echo -n $(while true; do echo -n 0; done | head -c "$2")$(dec_to_hex "$1") | tail -c "$2"
}

escape() {
	#Adds \x escapes to hexdump string

	while read -n 2 i; do
		[ -n "$i" ] && echo -n '\x'"$i";
	done
}

dnsmasq() {
	#Converts \x00\x00 notation to 00:00 notation

	sed -e 's/^\\x//g' | sed -e 's/\\x/:/g'
}

dhcp_option() {
	#Compile DHCP option using option number and data.
	#Generates hex string including header with option number and data length.

	separator='\x'
	option_id="$1"
	option_data="$2"

	len=$(echo -n "$option_data" | wc -c)
	dec_to_hex_right $option_id 2 | escape
	dec_to_hex_right $len 2 | escape
	printf "$option_data" | xxd -ps -c 256 | escape
}

pd_prefix() {
	#Generate prefix sub-option to be included in PD option of DHCPv6

	option_id=26
	lifetime_preferred='\x00\x00\x01\x2C'
	lifetime_valid='\x00\x00\x01\x2C'

	prefix_length='64'
	#prefix = 16 octets:
	prefix='\x20\x01\x06\x7c\x21\x90\x1a\x01''\x00\x00\x00\x00\x00\x00\x00\x00'

	prefix_length_hex=$(dec_to_hex $prefix_length)

	#echo $prefix_length_hex
	dhcp_option $option_id "$lifetime_preferred$lifetime_valid$prefix_length_hex$prefix"
}

pd() {
	#Generate data of PF option of DHCPv6

	iaid='\x00\x00\x00\x00'
	t1='\x00\x00\x01\x2C'
	t2='\x00\x00\x01\x2C'
	ia_pd_opts="$(pd_prefix)"
	echo -n "$iaid$t1$t2$ia_pd_opts"
}

pd_option() {
	#Generate PD option of DHCPv6 (including header)

	option_id=25
	dhcp_option $option_id "$(pd)"
}

pd | dnsmasq
echo
