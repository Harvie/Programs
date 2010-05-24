#!/usr/bin/php
<?php
echo("Ahoj svetee!\n");

$array = array("Neo", "Trinity", "Morpheus");
foreach($array as $_) echo( md5($_).": $_\n");

for($i=1;$i<100000;$i++) { print(md5($i*rand(0,9))."\r"); }

echo "\n";

