<?php
//Harvie's MAC sniffing toolkit (2oo7)
//Vice informaci cesky: https://www.soom.cz/articles/print.php?aid=406

/*
This if primary for MS Windows (may work at other system, depending on 3rd side programs' output)
3rd side programs:
- ping
- arp
- ngrep (requires WinPCap for Windows or LibPCap for Unixs)
*/

///SETTINGS/////////////////////////////////////
$ngrep = "ngrep"; //NGREP binary
$ping = "ping -n 1"; //PING with arguments
$arp = "arp -a"; //ARP with arguments to show all ARP records

///FUNCTIONS////////////////////////////////////

//Get HW (MAC) address from IP address
function get_mac($ip) {
  $ip = trim($ip);
  shell_exec($GLOBALS["ping"]." ".$ip);
  $arp = shell_exec($GLOBALS["arp"]);
  $arp = explode("\n", $arp);
  foreach($arp as $line) {
    if(ereg(": $ip ---", $line)) { return("This is your adapter, to find MAC try \"ipconfig /all\""); }
    if(ereg("  $ip ", $line)) {
      //echo($line."\n"); //Debug
      $line = explode($ip, $line);
      $line = trim($line[1]);
      $line = explode("dynamic", $line);
      $line = trim($line[0]);
      //echo($line."\n"); //Debug
      return($line);
    }
  }
  return("Not found. Couldn't broadcast to IP.");
}

//Passive scan for active computers (IPs) in network (it's 100% stealth),
//but you can use "nmap" (for example) for scanning more more quickly and efectively...
//This is waiting in infinite loop...
function sniff_ips($device = 1, $subnet = "") {
  $device = trim($device);
  $subnet = trim($subnet);
  $ngrep = ($GLOBALS["ngrep"]." -d ".$device);
  $fp = popen($ngrep, "r");
  
  $ips[0] = "";
  $i = 0;
  while($fp && !feof($fp)) {
    $line = fgets($fp);
    if(ereg("$subnet.*:.* -> .*:.*", $line)) {
      $line = explode(" ", $line);
      $line = explode(":", $line[1]);
      $ip = trim($line[0]);
      
      if(!in_array($ip, $ips)) {
        $ips[$i] = $ip;
        $i++;
        
        //You have $ip, you can do anything, that you want:
        echo($ip." = ".get_mac($ip)."\n"); //Get it's MAC and print it
        
      }
    }
  }
}

//Quick active scan for MACs and IPS
function quick_ipmac_scan($subnet = "192.168.1") {
  for($i=1;$i<256;$i++) {
    //Mega threaded ( This will open 255 processes ;))
    $fp[$i] = popen($GLOBALS["ping"]." ".$subnet.".".$i, "r");
  }
  for($i=1;$i<256;$i++) {
    while( $fp[$i] && !feof($fp[$i]) ) { fgets($fp[$i]); }
  }
  system($GLOBALS["arp"]);
}

///Examples of usage://///////////////////////////////////////////////////////
//You have to modify this script, to get that output format, that you want...


//Sniff for IPs:
echo("Sniffing for IP/MAC addresses\nC-c for stop\n\n");
//This will sniff on 3rd device ("ngrep -L" for device listing)
//And only IPs that starts with "192.168" will be accepted
sniff_ips(3, "192.168"); //ngrep -d 3 | grep 192.168.*:.* -> .*:.*

/*
Example output:
Sniffing for IP/MAC addresses
C-c for stop

192.168.15.82 = This is your adapter, to find MAC try "ipconfig /all"
192.168.15.65 = 00-00-24-c1-e7-e8
192.168.15.84 = 00-04-e2-cb-bc-6a
192.168.15.77 = Not found. Couldn't broadcast to IP.
192.168.15.80 = Not found. Couldn't broadcast to IP.
*/

//--------------------------------------------------------------------------


//Quick active scan for MACs/IPs:
echo("Scanning for IP/MAC addresses\nC-c for stop\n");
quick_ipmac_scan("192.168.1");

/*
Example output:
Scanning for IP/MAC addresses
C-c for stop

Rozhrani: 192.168.15.82 --- 0x40003
  internetova  adresa    fyzicka  adresa        typ
  192.168.15.65         00-00-24-c1-e7-e8     dynamicka 
  192.168.15.80         00-16-ce-0a-0e-a1     dynamicka 
*/

//--------------------------------------------------------------------------

//Get MAC:
$ip = "192.168.15.82"; //This is your adapter, to find MAC try "ipconfig /all"
$ip = "404.168.15.82"; //Not found. Couldn't broadcast to IP.
$ip = "192.168.15.65";
echo("IP: $ip\nMAC: ".get_mac($ip)."\n");

/*
Example output:
IP: 192.168.15.65
MAC: 00-00-24-c1-e7-e8
*/

?>
