#!/usr/bin/php
<?php
//HTML Mail parser 0.4
//<-Harvie 2oo7
/*
 *This thing checks URLs from STDIN and printing found E-Mails to STDOUT.
 *Use it well...
 */
 
 $in = fopen('php://stdin', 'r');
 while(!feof($in)) {
	$url = trim(fgets($in)); //echo($url); //Debug
	$page = @file($url);
	if(!$page) continue;
	foreach($page as $line) {
		if(!eregi('(mailto:|@)', $line)) continue;
		$line = spliti('mailto:', $line);
		array_shift($line);
		foreach($line as $mail) {
			$mail = spliti('(<|>| |\?|")', htmlspecialchars_decode($mail));
			echo(trim($mail[0])."\n");
		}
	}
 }