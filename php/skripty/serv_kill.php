<?php

echo("go go go!");

while(1) {
  
  $fp = fsockopen ("localhost", 8080, &$errno, &$errstr, .2);
  socket_set_timeout( $fp, 0, 1 );
  $err = 0;
  while($err == 0) {
    if ( !fwrite($fp, "KILLKILLKILLKILLKILLKILLKILLKILLKILLKILLKILLKILLKILLKILLKILLKILLKILLKILL") ) //GET / HTTP/1.1\r\n\r\n
      { $err = 1; }
  }
  fclose($fp);
}

?>
