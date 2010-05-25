<?php
/*

Maximalni mozna hodnota
a5m64tg2n48 == 37131929274566024
die(base_convert("a5m64tg2n48", 36, 10)."\n");

*/

$start = 4; //Minimalni pocet mist
$stop = 4; //Maximalni pocet mist

$start = pow(36, ($start - 1));
$stop = pow(36, $stop);

system("title GENERATING BRUTE FORCE SEQUENTION");
$i = $start;
while($i <= $stop) {
  $generated = base_convert($i, 10, 36);
  //Dalsi ukony (specialni znaky, ...)
  echo($generated."\n");
  $i++;
}

?>
