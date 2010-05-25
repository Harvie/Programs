<?php

$domena = "gmail.com";
$google = "com";
$file = fopen(("http://www.google.".$google."/search?num=100&q=+%22.%40".$domena."%22"), "r");
$text = "";

while(1) {
  echo(fgetc($file));
  //$text = ($text + fgetc($file));
  }
  
?>
