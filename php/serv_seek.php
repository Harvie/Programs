<?php
//Googlevulnbot

$q = urlencode("allinurl:iis"); //Google query string
$total = 1020; //Max results
$num = 100; //Results per page
$start = 0; //First result

if(isset($argv[1])) $q = urlencode($argv[1]);
while($start < $total) {
  $query = "http://www.google.cz/search?q=$q&num=$num&hl=cs&lr=&as_qdr=all&start=$start&sa=N";
  $start = $start + $num;
  preg_match_all('(http:\/\/[_a-zA-Z0-9\.\-]+\.[a-zA-Z]{2,4})',htmlspecialchars_decode(file_get_contents($query)), $response); //{1}[-_~&=\ ?\.a-z0-9\/]*
  $response = $response[0];
  unset($stack); $stack[0] = '';
  foreach($response as $addr) {
    $addr = str_replace('http://', '', $addr);
    $addr = trim($addr);	
    if (!eregi("(209\.85\.135\.104|google|<|>|htm|php|gif|jpg)", $addr) && !in_array($addr, $stack) ) {
      echo($addr."\n");
      array_push($stack, $addr);
    }
  }
}