<?php

$pfp = popen ("cmd.exe", "rw");
$ifp = fopen ("php://stdin", "r");

while($pfp) {
  echo (fgetc($pfp));
  if (feof($ifp)) { fwrite ($pfp, fgetc($ifp)); }
}

?>
