#!/usr/bin/php
<?php
//CrawlB00st 0.1 (filter for crawlers)
//<-Harvie 2oo7
/*
 *This thing will boost crawling of web.
 *Use it well...
 */
 
 $stdin = fopen('php://stdin', 'r');
 while(!feof($stdin)) {
	$url = trim(fgets($stdin)); //echo($url); //Debug 
	echo("$url\n");
	$in = @file($url); if(!$in || !is_array($in)) return(1);
	foreach($in as $line) {
		$line = spliti('href="http://', $line);
		if(sizeof($line) > 1) {
			array_shift($line); //print_r($line); //Debug
			foreach($line as $nurl) {
				$nurl = spliti('(\?|#|\*|")', $nurl);
				$nurl = 'http://'.trim(htmlspecialchars_decode($nurl[0])); //echo($nurl."\n"); //Debug
				$test = @fopen($nurl, 'r');
				if($test) echo("$nurl\n");
			}
		}
	}
}

