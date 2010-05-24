#!/usr/bin/php
<?php
//HSp33der Web Crawler 4.1
//<-Harvie 2oo7
/*
 *This thing crawls the web and printing found URLs to STDOUT.
 *Use it well...
 */

//Basic settings
$first_url = 'http://harvie.stokoruna.cz';
$first_url = 'http://harvie.ath.cx';
$first_url = 'http://www.google.cz/search?q=web';
$first_url = 'http://www.google.com/search?as_q=www&num=10000';
$first_url = 'http://jyxo.cz/s?q=%23linux+format%3Ahtml&d=cz&cnt=1000&o=noacc,nostem'; //I will start crawling here
//$first_url = 'http://ftp.mozilla.org/pub/mozilla.org/camino/releases/Camino-1.5.dmg'; //I will start crawling here
$maximum_file_size = 1000000; //Maximum filesize of downloaded page in bytes
$cache_size = 100; //Maximal cache size
$min_cache_size = 20; //Cache size after cleanup

//Filter settings
$eregi_url_blacklist = array(
	'(W3\.org|W3C)', //Hell knows...
	'(shop|xxx|porn|lesbian|hot)', //Commercial sites
	'(google|209.85.135.104|amazon.com|seznam.cz|centrum.cz|atlas.cz|zoznam.sk|quick.cz)', //Big sites
	'.\.(css|ico|gif|jpg|png|bmp|cgi)', //Misc. webpage content
	'.\.(avi|mpg|mpeg|mov|wmv|wm|mp3|ogg|wma)', //Multimedia files
	'.\.(pdf|swf|flv|cfm)', //Other text files
	'.\.(exe|zip|rar|gz|bz|bz2|tar)' //Big and binary files
);
$eregi_url_whitelist = array(
	'.\.(html|htm|shtml|php|xml|jsp|asp)' //HTML files only (Whitelisting disabled by default...)
);

//Development settings
//$debug = true;

//Init
$url_db = array($first_url);
$i = 0;
$total = 0;

//Functions

function file_size_check($file, $max, $block = 256) { //If $file is bigger than $max, returns 0 else 1
	if($GLOBALS['debug']) echo("*FileSizeCheck!: $file\n");
	$fp = @fopen($file, 'r');
	if(!$fp) {
		if($GLOBALS['debug']) echo("**Can't open!!!: $file\n");
		return(0);
	}
	$dl = 0;
	while(!feof($fp)) {
		$dl = $dl+strlen(fgets($fp, $block)); //echo("$dl\n"); //Debug
		if($dl > $max) {
			fclose($fp);
			if($GLOBALS['debug']) echo("**Too much big file!!!: $file\n");
			return(0);
		}
	}
	fclose($fp);
	return(1);
}
function check_crawl_url($url) { //URL Filter for crawl_url function
	foreach($GLOBALS['eregi_url_blacklist'] as $black_url) {
		if(eregi($black_url, $url)) return(0);
	}
	if(in_array($url, $GLOBALS['url_db'])) return(0);
	if(!file_size_check($url, $GLOBALS['maximum_file_size'])) return(0);
	foreach($GLOBALS['eregi_url_whitelist'] as $white_url) {
		if(eregi($white_url, $url)) return(1);
	}
	return(1); //1 == disable whitelisting, 0 == enable whitelisting
}

function found_url($url) { //What to do with found URL
	$test = @fopen($url, 'r');
	if(!$test) {
		if($GLOBALS['debug']) echo("> Can't open file!: $url\n");
		return(1);
	}
	
	echo($url."\n");
}

function crawl_url($url) { //Recursive crawler (with cleanup of course...)
	found_url($url);
	if($GLOBALS['i'] >= $GLOBALS['cache_size']) return(0);
	$in = @file($url); if(!$in || !is_array($in)) return(1);
	foreach($in as $line) {
		$line = spliti('href="http://', $line);
		if(sizeof($line) > 1) {
			array_shift($line); //print_r($line); //Debug
			foreach($line as $nurl) {
				$nurl = spliti('(\?|#|\*|")', $nurl);
				$nurl = 'http://'.trim(htmlspecialchars_decode($nurl[0])); //echo($nurl."\n"); //Debug
				if(check_crawl_url($nurl)) {
					array_push($GLOBALS['url_db'], $nurl);
					$GLOBALS['i']++; $GLOBALS['total']++;
					if($GLOBALS['debug']) echo("-cache: ".$GLOBALS['i']." +total urls crawled: ".$GLOBALS['total']."\n"); //Debug
					if($GLOBALS['i'] < $GLOBALS['cache_size']) {
						crawl_url($nurl);
					}
					if($GLOBALS['i'] >= $GLOBALS['cache_size']) return(0);
				}
			}
		}
	}
}


//CODE (Cleanup for crawl_url())
if($debug) echo("!!! DEBUG MODE ON !!!\n");
while(1) {
	if($debug) echo("\n+Strating with: ".$url_db[0]."\n"); //Debug
	foreach($url_db as $url) {
		if($i < $cache_size) crawl_url($url);
	}
	//Cache cleanup
	if($debug) echo("!Cache Cleanup\n"); //Debug
	while(sizeof($url_db) > $min_cache_size) {
		array_shift($url_db);
	}
	$url_db = array_reverse($url_db);
	$i = $min_cache_size;
}