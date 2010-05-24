#!/usr/bin/php
<?php
$fp = fopen("http://www.root.cz/", "r");

while($line = fgets($fp)) {
  if(ereg("<h3><a href=\"/clanky/", $line)) {
    $line = explode("<h3><a href=\"/clanky/", $line);
    $line = explode("/\">", $line[1]);
    echo($line[0]."\n");
  }
}
