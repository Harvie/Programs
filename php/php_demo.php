#!/usr/bin/php
<?php
echo("Special chars example\n<-Harvie 2oo7\n\n");
$tocse = "-\|/";

while(1) {

echo("INIT: ..........\x0d");
for($i=0;$i<11;$i++) {
  usleep(100000);
  echo("INIT: ");
  for($x=$i;$x>0;$x--) echo("#");
  echo("\x0d");
}

$tnum = rand(0,99);
echo("Find: ".($find = md5($tnum)).":??\n");
$i = 0;
while(1) {
  $tnum = rand(0,99);
  $test = md5($tnum);
  echo("\x0d MD5: ".$test." ".$tocse[$i]." :$tnum   ");
  usleep(50000);
  if($test == $find) { break; }
  $i++;
  if($i > 3) { $i = 0; }
}
echo("\x0d MD5: ".$test.":$tnum          \n Found\x07!!!\n\n");

}

