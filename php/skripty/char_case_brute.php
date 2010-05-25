<?php

//die(urldecode("http://www.aliveband.se/L%e5tar/07-HARD%20DAYS%20NIGHT.mp3"));

$str = "Lo";


///CODE//////////////////
function str2lower($str) {
  return strtolower($str);
}
function str2upper($str) {
  return strtoupper($str);
}

function fill_notset($str, $len, $char) {
  $strout = $str;
  $start = strlen($strout);
  for($i=$start;$i<$len;$i++) {
    $strout[$i] = $char;
  }
  return($strout);
}
//echo(fill_notset("aa", 5, "b")); die();
/////////////////////////
$origstr = $str = str2lower($str);
$len = strlen($str);
$origlen = $len;

$mask = "";
for (;$len > 0;$len--) {
  $mask = $mask."1";
} //echo($mask);

$i = base_convert($mask, 2, 10); //echo($i);
while(1) {
  $mask = base_convert($i, 10, 2);
  $mask = fill_notset($mask, $len, "b");
  if(strlen($mask) > strlen($str) || $i < 0) { die(); }
  
  //Regular
  $str = $origstr;
  for ($len = $origlen-1;$len>=0;$len--) {
    if(isset($mask[$len]) && $mask[$len] == "1") {
      $str[$len] = str2upper($str[$len]);
    }
  } echo($mask);
  echo("$str\n");
  
  /*/Reversed
  $mask = str_replace(array("0","1"), array("1","0"), $mask);
  $str = $origstr;
  for ($len = $origlen-1;$len>=0;$len--) {
    if(isset($mask[$len]) && $mask[$len] == "1") {
      $str[$len] = str2upper($str[$len]);
    }
  } //echo($mask);
  echo("$str\n");
  */

$i--;
}
