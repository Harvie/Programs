#!/usr/bin/php
<?php

function ip_rand($all = false) {
	if(!$all) {
		return(rand(1,254).'.'.rand(1,254).'.'.rand(1,254).'.'.rand(1,254));
	} else {
		return(rand(0,255).'.'.rand(0,255).'.'.rand(0,255).'.'.rand(0,255));
	}
}

function ip_range($ip) {
	$ip = explode('.', trim($ip));
	if($ip[0]==0 || $ip[0]==255) return 3;
	if($ip[0]==127 && $ip[1]==0 && $ip[2]==0) return 2;
	if($ip[0]==10) return 1;
	if($ip[0]==172 && $ip[1]>=16 && $ip[1]<=31) return 1;
	if($ip[0]==192 && $ip[1]==168) return 1;
	return 0;
}

function scan_check($ip, $port=80, $timeout = 0.4) {
	$fp = @fsockopen($ip, $port, $errno, $errstr, $timeout);
	if($fp) return 1;
}

//while(1) echo(ip_range(ip_rand())."\n");


while(1) {
	if(ip_range($ip = ip_rand())) continue;
	if(scan_check($ip, 23)) echo($ip."\n");
	//echo($ip.' - '.gethostbyaddr($ip)."\n"); //Debug
}
/*
24-bit block 	10.0.0.0 - 10.255.255.255
20-bit block 	172.16.0.0 - 172.31.255.255
16-bit block 	192.168.0.0 - 192.168.255.255
*/