<?php
$tocse = "-\|/";
$i = 0;
while(1) {
  for($x=0;$x<10000;$x++) echo($tocse[$i]."\x0d");
  $i++;
  if($i > 3) { $i = 0; }
}
