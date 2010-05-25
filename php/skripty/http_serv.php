<?php

// set some variables
$host = "localhost";
$port = 81;

$header = 
  "HTTP/1.0 200 OK\n".
  "Server: Harvie's HTTPd\n".
  "Connection: close\n\n";

/*
HTTP/1.0 200 OK
Date: Thu, 05 Jul 2007 17:28:16 GMT
Server: Apache/2.0.59 (Win32) PHP/5.1.4
Content-Length: 5568
Connection: close
Content-Type: text/html;charset=utf-8
*/

// don't timeout!
set_time_limit(0);

while(1) {

$socket = socket_create(AF_INET, SOCK_STREAM, 0) or die("Could not create socket\n");
$result = socket_bind($socket, $host, $port) or die("Could not bind to socket\n");
$result = socket_listen($socket, 3) or die("Could not set up socket listener\n");

// accept incoming connections
// spawn another socket to handle communication
$spawn = socket_accept($socket) or die("Could not accept incoming connection\n");

$loop = 1;
while($line = socket_read($spawn, 1024) && $loop) {
  echo($line);
  if(eregi("(GET|POST)", $line)) {
    $line = trim($line);
    echo($line);
    $line = explode(" ", $line);
    readfile(".".$line[1]);
    $loop = 0;
  }
}

// read client input
//$input = socket_read($spawn, 1024) or die("Could not read input\n");
// clean up input string
//$input = trim($input);
// reverse client input and send back
//$output = strrev($input) . "\n";
//socket_write($spawn, $output, strlen ($output)) or die("Could not write output\n");

// close sockets
socket_close($spawn);
socket_close($socket);

}

?>

