<?php
//This will draw the ASCII neverending sinus curve.
//Writen by (c) Harvie in 2oo6
//This is script for PHP CLI (Not WebPage).
//You can port it to web by replacing "/n" by "<br>/n"

//Settings:
$start = 0;     //0 //First X
$step = 0.1;    //0.1 //Increase X on every line by this number
$reset = (2*PI()); //When X >= reset, then it will be reseted to zero
$zoom = 1;      //1 //This will repeat every line few times
$offset = 1.1;    //1.1 //1 //0 -> Zero offset will draw only half of sin curve.
$width = 35;     //20 //35 //40 //This will stretch every line.
$sleep = "15000";   //15000 - Wait between lines in microseconds
$line = (" ");     // " " // ":" //chr ( 176 ) //This is the string, that will be repeated from display start to the curve (curve inlay).
$endline = ("+\n"); // "#\n" // ".\n" //chr ( 219 )."\n" //This is the string, that will be printed at end of every line (curve outline).

//Code:
echo ("[SinusCurve]-[c][Harvie][2oo6]\n\n");
echo ("0|                                           y\n");
echo ("-|--------------------------------------------->\n");
echo ("x|\n");
echo (" V\n\n");

sleep(2);

$pos = $start;
while(1) {

  usleep($sleep); //Wait in microseconds
  
  $znaku = ((sin($pos) + $offset) * $width);
  
  $zoom2 = $zoom;
  while ($zoom2 > 0) {
  
    $znaku2 = round($znaku); //It looks better after round ;)
    while ($znaku2 > 0) {
      echo $line;
      $znaku2 = ($znaku2 - 1);
    }
    //echo(" ".$pos." "); //Debug
    echo ($endline);
    
    $zoom2 = ($zoom2 - 1);
  }
  
  //echo(" ".$pos." "); //Debug
  $pos = ($pos + $step);
  if ($pos >= $reset) {
    $pos = 0;
  }

}
?>
