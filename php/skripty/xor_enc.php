<?php

function bin_trim ($bin) {

  $x = (8 - strlen($bin));
  $y = "";
  while($x) {
   $y = ($y."0");
   $x--;
  }
  return($y.$bin);
  
}

function xor_bytes ($bina, $binb) {
  $bin = "";
  $x = 8;
  while($x) {
    if ($bina[$x-1] XOR $binb[$x-1])
      { $bin = ( 1 . $bin ); } else { $bin = ( 0 . $bin ); }
    $x--;
  }
  return $bin;
}

//CODE/////////////////////////////////////////////////////

$message = "POKUS CISLO JEDNA DVA TRI";
$message = ")-#&O&$6(.:E/-:H&#&*H--$.";
$message = "message MESSAGE toto je pokus 545446653šèøèøžøýá ahoj";
//$message = "%-) -g&3N%Z‹˜F—ƒŽÍÜÝA×CÔI{¼ªÝÌÛÌ¸Õe‰“";
$message = implode("", file("cypher.txt"));
$code = "HHhgHGHGkGBCSEvnkJizzáý76øèèýíé09éuáuázáTR54E4E4Ezázùdlmkdsfh-";

//print "Pocet parametru: " . $argc . "\n";
//foreach($argv as $param)
//    print $param . "\n";

//if($argc != 3) { die("Usage: xor.php secretcode in.txt > out.txt\n"); }

//echo($argv[0]."\n");
//$code = $argv[1];
//$message = implode("", file($argv[2]));
$message = "&.(h(-fKH-)!F
g%«ÍH$,7
Y";
$code = "gfgFHJghGjhghjfVHGhjGhjGvvXfgdTGfHgHfgFhfVBGcftDtFzdfdgfgf6ztFzF";



//Message to bin array:
$i = 0;
$message_bin = "";
while($i < strlen($message)) {
  $message_bin = ($message_bin . bin_trim(base_convert(ord($message[$i]),10,2)). "\n " );
  $i++;
}
//echo ($message_bin."\n\n"); //Debug

$message_bin = explode(" ",$message_bin);

//Encode:
$i = 0;
$y = 0;
while ($i < strlen($message)) {

  $char = $message_bin[$i];
  $cchar = bin_trim( base_convert( ord($code[$y]), 10, 2) );
  
  //echo($code[$y]); //Debug
  //echo($cchar."\n"); //Debug
  //echo($char); //Debug
  
  $message[$i] = chr( base_convert(xor_bytes($char, $cchar),2,10 ));
  
  $y++;
  if ($y >= strlen($code)) { $y = 0; }
  $i++;
  
}

echo($message);

?>
