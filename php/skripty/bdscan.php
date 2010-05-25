#!/usr/bin/php
<?php
//BDScan
//Harvie 2oo7

///SETTINGS////////////////////////////////////////////
$host = "localhost";
//$host = "192.168.2.1";
$timeout = 0.9;

if(isset($argv[1])) $host = $argv[1]; //Host
if(isset($argv[2])) $timeout = $argv[2]; //Timeout

///PORTLIST////////////////////////////////////////////
$ports = array (
//#PORT => "CLASS.Name",
  21    => "SRV.FTP Service",
  22    => "SRV.Secure Shell Service",
  23    => "SRV.Telnet Service",
  24    => "BD.Harvie's NetCat BackDoor",
  80    => "SRV.HTTP Service",
  8080  => "SRV.HTTP Service",
  
  113   => "WRM.Korgo.F",
  903   => "BD.NetDevil",
  1080  => "WRM.MyDoom.B, MyDoom.F, MyDoom.G, MyDoom.H",
  2283  => "WRM.Dumaru.Y",
  2535  => "WRM.Beagle.W, Beagle.X, other Beagle/Bagle variants",
  2745  => "WRM.Bagle",
  3067  => "WRM.Korgo.F",
  3127  => "WRM.My_Doom.A",
  3128  => "WRM.MyDoom.B",
  3140  => "BD.OptixPro.13 and variants",
  5554  => "WRM.Sasser through Sasser.C, Sasser.F",
  6129  => "RM.DameWare",
  8866  => "WRMBeagle.B",
  9898  => "WRM.Dabber.A and Dabber.B",
  9996  => "WRM.Sasser",
  10000 => "WRM.Dumaru.Y",
  10080 => "WRM.MyDoom.B",
  12345 => "BD.NetBus",
  16661 => "BD.HaxDoor",
  17300 => "BD.Kuang, Kuang2",
  27347 => "BD.SubSeven",
  27374 => "BD.SubSeven",
  65506 => "BOT.PhatBot, Agobot, Gaobot"
  );

///SCAN///////////////////////////////////////////////////////
echo("Harvie's BackDoor PortScanner\n");
echo("[u] Usage: bdscan [host] [timeout]\n");
echo("[i] Timeout is set to $timeout s\n");
echo("[i] Scanning $host for backdoors and basic services...\n\n");

foreach($ports as $port => $bd) {
  $sock = @pfsockopen($host, $port, $errno, $err, $timeout);
  if($sock) {
    echo("[!] $port : $bd\n");
    $found = true;
  } else {
    //echo("[-] $port : $bd\n");
  }
}

if(isset($found)) {
  echo("\n[!] Services listed above found...\n");
} else {
  echo("[i] No BackDoors/services found...\n");
}
echo("[i] Finished...\n");

///SUX_PARSER/////////////////////////////////////////////////
/*
$fp = fopen("bdports.txt", "r");
while(!feof($fp)) {
  $line = trim(fgets($fp));
  $line = explode("\t", $line, 2);
  $line[0] = trim($line[0]);
  $line[1] = trim($line[1]);
  //print_r($line);
  echo("  ".$line[0]." => \"".$line[1]."\",\r\n");
}
*/
