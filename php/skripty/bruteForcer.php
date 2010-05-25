<?php

//CHAR AT POSITION "X"
function charAt($str, $pos) {

$pos = ($pos - 1); //Offset
return (substr($str, $pos, 1)) ? substr($str, $pos, 1) : -1;

}

//NEXT CHARACTER
function nextChar ( $in ) {

$chars = ("abcdefghijklmnopqrstuvwxyz "); // Ends with space
//echo(charAt($chars, 7)); //Debug
$len = (strlen($chars));

$char = $in;

$charpos = ( strpos($chars, $char) );
$newcharpos = ($charpos + 1);
$char = $chars[$newcharpos];

$out = $char;
return ($out); //Returns "" after last character.

}

//BRUTE FORCER
function bruteForce ( $word ) {

$first = ("a");
$prelast = ("z");
$last = (" ");

$wpos = 0;

while ( $word[$wpos] == $prelast ) {
  $wpos = ($wpos + 1);
}

$word[$wpos] = nextChar($word[$wpos]);

/*
while ( $word[$wpos] != $prelast ) {
  $word[$wpos] = nextChar($word[$wpos]);
  if ( $word[$wpos] == $last ) {
    $word[$wpos] = $prelast;
    }
}
*/

return $word;
}



//===CODE===

//$test = ("nazdar");
//echo ($test["2"]);
//die();

echo(nextChar ("d") . "\n");
echo(bruteForce ("ahoj") . "\n----------------------\n\n");

$ted = ("a");
while (1) {
echo($ted."\n");
$ted = bruteForce($ted);
}

/*
$hash = ("79c2b46ce2594ecbcb5b73e928345492");
$pass = ("ahoj");
//echo(md5("ahoj"));

$loop = 1;
while($loop) {

$now = $pass;

  if ( md5($now) == $hash ) {
    die ("Hash: $hash\nVysledek: $now\n");
  }

}
*/

?>
