<?php
//This will draw the ASCII neverending sinus curve.
//Writen by (c) Harvie in 2oo6
//This is script for PHP CLI (Not WebPage).
//You can port it to web by replacing "/n" by "<br>/n"

//Settings:
$start = -3;         //0 //First X
$zero = 0;           //Zero line
$stop = 0.5;           //Last X
$step = 0.1;         //0.1 //Increase X on every line by this number
$reset = "0";        //When X >= reset, then it will be reseted to $resetto (use "" to disable reset).
$resetto = "0.1";    //$resetto
$zoom = 1;           //1 //This will repeat every line few times
$sleep = "15000";    //15000 - Wait between lines in microseconds
$line = (" ");       // " " // ":" //chr ( 176 ) //This is the string, that will be repeated from display start to the curve (curve inlay).
$endline = ("+\n");  // "#\n" // ".\n" //chr ( 219 )."\n" //This is the string, that will be printed at end of every line (curve outline).

//Function:
//You can specify your function here:
function func($x) {

$offset = 20;    //1.1 //1 //0 -> Zero offset will draw only half of sin curve.
$width = 1;     //20 //35 //40 //This will stretch every line.

$y = ( ( (3 / $x) + $offset) * $width );
//echo("Debug: ".$x." ".$y." "); //Debug
return ($y);

}

//Code:
echo ("[SinusCurve]-[c][Harvie][2oo6]\n\n");
echo (" |                                           y\n");
echo ("-|--------------------------------------------->\n");
echo ("x|\n");
echo (" V\n\n");

sleep(2);

//Set x to $start:
$pos = $start;

//Drawing:
while(1) {

  //usleep($sleep); //Wait in microseconds
  
  $znaku = func($pos);
  
  $zoom2 = $zoom;
  while ($zoom2 > 0) {
  
    $znaku2 = round($znaku); //It looks better after round ;)
    while ($znaku2 > 0) {
      echo $line;
      $znaku2 = ($znaku2 - 1);
    }
    //echo(" ".$pos); //Debug
    echo ($endline);
    
    $zoom2 = ($zoom2 - 1);
  }
  
  //Draw: Y axis:
    //echo(" ".$pos."\n"); //Debug
  if ($pos == $zero) {
    echo("-+------------------------------------------------------------>\n");
  }
  
  //Increase X:
  $pos = ($pos + $step);
  
  //Reset (for neverending curves, etc...):
  if ( ($pos >= $reset) && ($reset != "") ) {
    $pos = $resetto;
  }
  
  //Stop if $stop is reached:
  if ($pos > $stop) {
    die();
  }

}
?>
