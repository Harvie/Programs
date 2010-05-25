<?php

function readline() {
    $fp = fopen("php://stdin","r");
    $line = fgets($fp, 1000);
    fclose($fp);
    return $line;
}

while(1){
  print ("\nHARVIE's CMDLine:\n");
  $cmd = ("");
  $cmd = trim(readline());
  if ( $cmd != "" ){
    system ($cmd);
  } else {
    while(1){
      $cmd = trim(readline());
    }
  }
}

?>
