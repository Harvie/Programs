<?php

$adv = "";
$fp = fopen("$adv.txt", "w+");
$pos = "0";

while( strpos($line, "File not found") == 0 ) {
$line = trim( implode(" ", file("http://irc.honemsetodouc.com/koucka/adventure/$adv/$pos.txt")) );
//$line = $str_replace("", "", $line);
fwrite($fp, $line."\r\n");
echo ($line."\r\n");
$pos++;
}

fclose($fp);
