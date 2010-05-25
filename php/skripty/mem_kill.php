<?php
$time = 10; //9

/*
$i = 0;
while(1) {
 $x[$i] = "1";
 $i++;
}
*/

echo("\nOPTIMIZING MEMORY");
echo("\nPLEASE WAIT...");

$i = 0;
$y = 0;
$t = time();
while(1) {
  while ( $y < 10 ) {
    $x[$i] = "KILL";
    $i++;
    $y++;
  }
    $y = 0;
  if ( time() > ($t + $time) )
   { echo("\nDONE!\nRELASING MEMORY FOR SYSTEM..."); die(); break; }
}
?>
