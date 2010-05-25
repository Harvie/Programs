#!/usr/bin/php
<?php
//HSp33der Web Crawler 5.1
//<-Harvie 2oo7
/*
 *This thing crawls the web and printing found URLs to STDOUT.
 *New technology!!!
 *Use it well...
 */

//Basic settings
$crawl_file = 'crawl.txt';
$lastc_file = 'last.txt';
$nextc_file = 'next.txt';
$seed_url   = 'http://jyxo.cz/s?q=%23linux+format%3Ahtml&d=cz&cnt=3&o=noacc,nostem';
$maximum_file_size = 500000; //Maximum filesize of downloaded page in bytes
//$debug = true;
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

//Functions
function in_file($string, $file) { //Like idn_array(), but with file
	if($GLOBALS['debug']) echo("in_file\n");
	$in = fopen($GLOBALS['crawl_file'], 'r');
	$string = trim($string);
	$len = strlen($string);
	while(!feof($in)) {
		$line = trim(fgets($in));
		//if( ($len == strlen($line)) && (strncasecmp($string, $line, $len) == 0) ) return(1);
		if( ($len == strlen($line)) && (eregi($string, $line)) ) return(1);
	}
	fclose($in);
	return(0);
}

function file_size_check($file, $max, $block = 250) { //If $file is bigger than $max, returns 0 else 1
	if($GLOBALS['debug']) echo("fscheck\n");
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
	@fclose($fp);
	return(1);
}
function check_crawl_url($url) { //URL Filter for crawl_url function
	if($GLOBALS['debug']) echo("check_crawl_url\n");
	foreach($GLOBALS['eregi_url_blacklist'] as $black_url) {
		if(eregi($black_url, $url)) return(0);
	}
	
	@fclose($GLOBALS['total']);
	if(in_file($url, $GLOBALS['crawl_file'])) return(0);
	$GLOBALS['total'] = fopen($GLOBALS['crawl_file'], 'a');
	
	if(!file_size_check($url, $GLOBALS['maximum_file_size'])) return(0);
	return(1); //1 == disable whitelisting, 0 == enable whitelisting
}

function save_crawled_url($url) { //Saves URL to database, etc...
	if($GLOBALS['debug']) echo("save_crawled_url\n");
	$GLOBALS['total_urls']++;
	$url = trim($url);
	echo($GLOBALS['total_urls'].':');
	echo($url."\n");
	@fwrite($GLOBALS['total'], $url."\n");
	@fwrite($GLOBALS['next'], $url."\n");
}

function crawl_url_once($url) { //Main crawling function
	if($GLOBALS['debug']) echo("crawl_url_once\n");
	save_crawled_url($url);
	$in = @file($url); if(!$in || !is_array($in)) return(1);
	foreach($in as $line) {
		$line = spliti('href="http://', $line);
		if(sizeof($line) > 1) {
			array_shift($line); //print_r($line); //Debug
			foreach($line as $nurl) {
				$nurl = spliti('(\?|#|\*|")', $nurl);
				$nurl = 'http://'.trim(htmlspecialchars_decode($nurl[0])); //echo($nurl."\n"); //Debug
				if(check_crawl_url($nurl)) save_crawled_url($nurl);
			}
		}
	}
}




//CODE
if(@filesize($nextc_file) < 8) {
  $next = fopen($nextc_file, 'a');
  fwrite($next, $seed_url."\n");
  fclose($next);
}
if(is_file($nextc_file)) {
  @unlink($lastc_file);
  copy($nextc_file, $lastc_file);
  unlink($nextc_file);
}

$total_urls = 0;
while(1) {
  if($debug) echo("\n-LOOP\n");
  $last = fopen($lastc_file, 'r');
  $next = fopen($nextc_file, 'a');
  $total = fopen($crawl_file, 'a');
  while(!feof($last)) {
    $url = trim(fgets($last));
    crawl_url_once($url);
  }
  @fclose($crawl_file);
  @fclose($lastc_file);
  @fclose($nextc_file);
  unlink($lastc_file);
  copy($nextc_file, $lastc_file);
  unlink($nextc_file);
}


