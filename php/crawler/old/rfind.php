#!/usr/bin/php
<?php
//RFIFinder 0.1 (filter for crawlers)
//<-Harvie 2oo7
/*
 *This thing will find urls with expected RFI hole in pages specified by STDIN lines.
 *Use it well...
 */
 
 function is_rfi_positive($url) {
	$rfi_regexp = '(\?|&)(page|url|include|readfile|require|inc)=.*\.(html|htm|php|php.|txt)';
	return eregi($rfi_regexp, $url);
 }
 
 $stdin = fopen('php://stdin', 'r');
 while(!feof($stdin)) {
	$url = trim(fgets($stdin)); //echo($url); //Debug 
	if(is_rfi_positive($url)) echo("$url\n");
	$in = @file($url); if(!$in || !is_array($in)) continue;
	foreach($in as $line) {
		$line = spliti('href="http://', $line);
		if(sizeof($line) > 1) {
			array_shift($line); //print_r($line); //Debug
			foreach($line as $nurl) {
				//echo("#"); //debug
				$nurl = spliti('"', $nurl);
				$nurl = 'http://'.trim(htmlspecialchars_decode($nurl[0])); //echo($nurl."\n"); //Debug
				$test = @fopen($nurl, 'r');
				if(is_rfi_positive($nurl) && $test) echo("$nurl\n");
			}
		}
	}
}

