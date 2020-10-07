<?php

/**
 * Given IPv6 address and prefix creates array of * wildcard suffixed
 * addresses to be used for as reverse DNS records
 */
function ipv6_prefix2wildcards($address, $prefix) {

	//Parse ip and prefix
	echo("IPv6 prefix: ".$address."/".$prefix."\n");
	$addr = inet_pton ($address);
	$pref = $prefix;
	$a = gmp_import($addr);

	//Separate maskable octets
	$pref_hex = ceil($pref / 4)*4; //Prefix rounded up to full octets
	$pref_rem = $pref_hex - $pref; //Remainder of prefix
	echo("Rounding prefix: ".$pref_hex."-".$pref_rem."\n");

	$a = gmp_div_q($a, gmp_pow(2,128-$prefix)); //Truncate to just prefix
	$a = gmp_mul($a, gmp_pow(2,$pref_rem)); //Fill remaining bits in last wildcard octet with zeros

	//Generate wildcards
	$wildcards = [];
	for($i = 0; $i < (2**$pref_rem) ; $i++) {
		//echo("ADDED: ".$i." = ".decbin($i)."\n");
		$ap = gmp_or($a, gmp_init($i));
		$wildcards[] = $ap;
	}

	return $wildcards;
}

if(isset($argv[2])) {
	$wcs = ipv6_prefix2wildcards($argv[1],$argv[2]);
} else {
	$wcs = ipv6_prefix2wildcards('2001:1:3::10', 67);
}

echo("\n");
foreach($wcs as $w) {
  //echo(gmp_strval($w, 2)."* :-)\n");
  echo(gmp_strval($w, 16)."*\n");
}
