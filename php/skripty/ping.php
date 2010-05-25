<?php

function ping($host) {
   $package = "\x08\x00\x19\x2f\x00\x00\x00\x00\x70\x69\x6e\x67";

   /* create the socket, the last '1' denotes ICMP */   
   $socket = socket_create(AF_INET, SOCK_RAW, 1);
  
   /* set socket receive timeout to 1 second */
   socket_set_option($socket, SOL_SOCKET, SO_RCVTIMEO, array("sec" => 3, "usec" => 0)); //<-- Change TimeOut Here <--
  
   /* connect to socket */
   socket_connect($socket, $host, null);
  
   /* record start time */
   list($start_usec, $start_sec) = explode(" ", microtime());
   $start_time = ((float) $start_usec + (float) $start_sec);
  
   socket_send($socket, $package, strlen($package), 0);
  
   if(@socket_read($socket, 255)) {
       list($end_usec, $end_sec) = explode(" ", microtime());
       $end_time = ((float) $end_usec + (float) $end_sec);
  
       $total_time = $end_time - $start_time;
      
       return $total_time;
   } else {
       return false;
   }
  
   socket_close($socket);
}

//READLINE
function readline ( $fp ) {
  //echo $prompt;
  
  $t = "";
  $c = "";
  
  while(1) {
    $c = fgetc($fp);
    if ($c == "\n") {    // && ($c == "\r") //May be later on oher systems
      return $t;
    } else {
      $t = ($t.$c);
    }
  }

   return $t;
    
}
//READLINE END

//Code

//echo (ping ("192.168.2.1"));
$fp = fopen("hosts.txt", "r+"); //host list file (hosts separated by newline, ends with two empty lines)

$fhost = "EMPTY";
while ($fhost != "") {

  $ping = "";
  $fhost = trim(readline($fp));
  
  if ( $fhost != "" ) {
    echo("HOST: ".$fhost);
    
    try {
      $ping = ping($fhost);
    } catch(string $err) {}
    
    if ( ($ping != "") && ($ping > "0") ) {
      echo(" - UP PING: ".$ping." sec.\n");
    } else {
      echo (" - TIMED OUT\n");
    }
  }
  
}

?>
