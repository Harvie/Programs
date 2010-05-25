<?php

$sss = stream_socket_server("tcp://127.0.0.1:81"); echo("A");
$sfp = stream_socket_accept($sss); echo("B");

while($sfp) {

  if (!feof($pfp)) {
    fwrite ($sfp, fgetc($pfp));
  }
  
  $char = fgets($sfp, 1000);
  if ($char != FALSE) {
    fwrite ($pfp, $char);
  }
  
}

echo("C");

?>
