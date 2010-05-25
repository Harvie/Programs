<?php

//Settings:
$c = 999; //Higher number = better PI

//Code:
$pico = 0;

$c1 = (180 / $c);
echo ("Debug1: ".$c1."\n");

$c2 = sin($c1);
echo ("Debug2: ".$c2."\n");

$pico = ( $c * $c2 );
echo("Short PI: " . PI() . "\n Counted PI:\n");
echo($pico);

?>
