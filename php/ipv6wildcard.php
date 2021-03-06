#!/usr/bin/php
<?php

/**
 * (c) 2020 - Tomas Mudrunka <harvie@github>
 *
 * Given IPv6 address and prefix creates array of * wildcard suffixed
 * addresses to be used for as reverse DNS records
 */
function ipv6_prefix2wildcards($address, $prefix) {
	//Special case of /0 prefix
	if($prefix == 0) return ['*'];

	//Parse ip and prefix
	echo("IPv6 prefix: ".$address."/".$prefix."\n");
	$addr = inet_pton ($address);
	$pref = $prefix;
	$a = gmp_import($addr);

	//Separate maskable octets
	$pref_hex = ceil($pref / 4); //Number of prefix octets
	$pref_bin = $pref_hex*4; //Number of prefix octets in bits
	$pref_rem = $pref_bin - $pref; //Remainder of prefix
	echo("Rounding prefix: ".$pref_bin."-".$pref_rem."\n");

	$a = gmp_div_q($a, gmp_pow(2,128-$prefix)); //Truncate to just prefix
	$a = gmp_mul($a, gmp_pow(2,$pref_rem)); //Fill remaining bits in last wildcard octet with zeros

	//Generate wildcards
	$wildcards = [];
	for($i = 0; $i < (2**$pref_rem) ; $i++) {
		//echo("ADDED: ".$i." = ".decbin($i)."\n");
		$ap = gmp_or($a, gmp_init($i));
		$w = gmp_strval($ap, 16);
		$w = str_repeat('0', $pref_hex-strlen($w)).$w;
		if(strlen($w) < 32) $w.='*';
		$wildcards[] = $w;
	}

	return $wildcards;
}

function ipv6_string2rev($str) {
	return implode('.',str_split(strrev($str)));
}

if(isset($argv[2])) {
	$wcs = ipv6_prefix2wildcards($argv[1],$argv[2]);
} else {
	$wcs = ipv6_prefix2wildcards('2001:1:3::10', 67);
}

echo("\n");
foreach($wcs as $w) {
	echo($w."\n");
}

echo("\n");
foreach($wcs as $w) {
	echo(ipv6_string2rev($w)."\n");
}
