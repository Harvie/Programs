<?php

$address = "google.cz";
$port = "80";

echo("Service: $address:$port Running: ");

$master = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
socket_set_option($master, SOL_SOCKET,SO_REUSEADDR, 1);
// socket_set_option($master, SOL_SOCKET, SO_SNDTIMEO, 10); //Pokus nastavit timeout

if (socket_connect($master, $address, $port)) { //Podařilo se inicializovat spojení?
  echo("1Yeah!\n"); //Ano
  socket_close($master);
} else {
  echo("1No!\n"); //Ne
}

if (socket_write ($master, "TEST")) { //Podařilo se inicializovat spojení?
  echo("2Yeah!\n"); //Ano
  socket_close($master);
} else {
  echo("2No!\n"); //Ne
}

?>
