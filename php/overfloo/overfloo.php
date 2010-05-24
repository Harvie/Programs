#!/usr/bin/php
<?php if(!isset($argv)) die('<pre>[!] FUCK-U-N0-0B!</pre>\n'); //.!.('.').!. No, this isn't a web app!
//Ov3rFl00
//Harvie 2oo7
/* Buffer-OverFlow VulnChecker
 * You can make template for any protocol
 */

//Settings
$template_dir = './flooz';
$floochar = 'A';
$addrstr = 'BBBB';
$start = 64;
$step = 2;
$max = 1024*1024;
$floostr = '%FLOOD%';
$lenstr = '%LENGHT%';
$host = 'stdout';
$port = 80;
$timeout = 3;

//C0DE
if($argc < 2) die(
	"[!] OverFloo".
	"Usage: overfloo template [host] [port] [max-len(^2)] [char] [addr]\n".
	"By: <--Harvie 2oo7 ;D\n"
);

if(isset($argv[1])) $template = $argv[1];
if(isset($argv[2])) $host = $argv[2];
if(isset($argv[3])) $port = $argv[3];
if(isset($argv[4])) $max = pow($argv[4],2);


function flood_make($len='64', $char='A', $end='BBBB') {
	for(;$len > 0;$len--) $str.=$char;
	return $str.$end;
}

//Init
$template = $template_dir.'/'.$template.'.floo';
echo(
	"[i] Testing: $host:$port\n".
	"[i] Using template: $template\n".
	"[i] Using range $start - $max B\n\n"
);
$template = @file_get_contents($template);
if(!$template) die("[!] Template not found!\n");
$tcp = ($host == 'stdout' || $host == '-');
//Loop
$len = $start;
while($len <= $max) {
	$data = str_replace($lenstr, $len, $template);
	$data = str_replace($floostr, flood_make($len, $floochar, $addrstr), $data);
	
	echo("[>] Sending: $len -> ");
	
	if($tcp) die("\n".$data);
	else {
		$fp = @fsockopen($host, $port, $errno, $err, $timeout);
		if(!$fp) die("\n[!] $host:$port DOWN ;D\n");
		fwrite($fp, $data); fflush($fp);
		
		if(!fwrite($fp, ';P')) {
			echo("Connection closed! ;D\n");
		} else {
			echo("OK!\n");
		}
		@fclose($fp);
	}
	
	$len *= 2;
}

if($tcp) {
	$fp = @pfsockopen($host, $port, $errno, $err, $timeout);
	if(!$fp) die("[!] $host:$port DOWN ;D\n");
}

echo("\n[X] Script exited succesfully!\n");

