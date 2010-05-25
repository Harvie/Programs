<?php

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

echo xor_bytes("10110000","10010000");

?>
