<?php

echo("[= HARVIE's MATRIX COUNTER =]\n");
srand(time());
sleep(2);
//chr ( 2530 )
$space = 0;

while (1) {
srand(100000 / microtime()); //1000 //100000

$rand = (rand()%11);
//ereg ("([10-20]{16,17,30,31,219,220,223,254,176,177,178})", $rand1, $rand);

switch ($rand) {
  case 10:
    $rand = " ";
    break;
  /*case 11:
    $rand = chr ( 219 );
    break;
  case 12:
    $rand = chr ( 176 );
    break;
  case 13:
    $rand = chr ( 177 );
    break;
  case 14:
    $rand = chr ( 178 );
    break;
  case 15:
    $rand = chr ( 31 );
    break;*/
}

//$space = 1;


/*
$space2 = 0;
while ($space2 < ($space / 1)) {
  echo(" ");
  $space2 = ($space2 + 1);
}
*/

$space = ($space + 1);
echo("$rand");


}

?>
