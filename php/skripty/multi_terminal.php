<?php

$i = 0;
$pfp[$i] = popen ("cmd.exe", "rw");
$ifp = fopen ("php://stdin", "r");

/*
a = 1
y = 25
*/

/*while(1) {
  echo( ord(fgetc($ifp))."-\n" );
}*/

while($pfp[$i]) {
  echo (fgetc($pfp[$i]));
  if (feof($ifp)) {
    $c = fgetc($ifp);
    if (ereg(chr(1), $c)) { $i++; $pfp[$i] = popen ("cmd.exe", "rw"); echo("\nconsole: $i\n"); }
    if (ereg(chr(25), $c)) { $i--; echo("\nconsole: $i\n"); }
    fwrite ($pfp[$i], $c);
  }
}

?>
