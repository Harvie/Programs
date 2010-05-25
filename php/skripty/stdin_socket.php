<?php

$sfp = fsockopen ("127.0.0.1", 24, &$errno, &$errstr, 5);
$ifp = fopen ("php://stdin", "r");

/*
while(1) {
echo(ord(fgetc($ifp)));
}
*/

while($sfp) {
  if (!feof($sfp)) { 
    echo (fgetc($sfp));
  }

  if (!feof($ifp)) { 
    fwrite ($sfp, fgetc($ifp));
  }
}

?>
