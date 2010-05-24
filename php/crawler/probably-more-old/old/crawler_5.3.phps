#!/usr/bin/php
<?php
//HSp33der Web Crawler 5.3
//<-Harvie 2oo7
/*
 * Description:
 * This script crawls the web and printing found URLs to STDOUT.
 *
 * Installation:
 * - Debian:
 *   # apt-get install php5-cli
 *   $ chmod +x crawler.php
 *   $ ./crawler.php (or $ php crawler.php)
 * - Windows:
 *   c:/>path/to/php5/php.exe crawler.php
 *   (you can add php.exe to PATH or associate .php files with it)
 *
 * Notes:
 * At this moment, there are few bugs in PHP5,
 * so this crawler may freeze after few minutes.
 * Check this for more info: http://bugs.php.net/bug.php?id=43098
 */

///SETTINGS
//Basic
$seed = 'http://www.secunia.com/'; //Start crawling from this page
$file = 'urls.txt'; //File to save crawled URLs (use sort to make URLs 100% exclusive)
$delete_url_db = true; //Delete file with saved URLs before crawling
//Advanced
$max_size = 10000; //How many bytes download from each page?
$max_urls = 30; //Size of URL buffer (prebuffered URLs to crawl, if full, no more URLs will be added to queue)
$buffer_increase = false; //Increase buffer at buffer underrun?
$random_url = false; //Select random urls from page? (This may override URL filter)
$history_max = 1000; //How many last URLs keep in history? (historied URLs will not be crawled again...)
//Misc.
ini_set('default_socket_timeout',1); //How long you want wait for a webserver? (seconds)
set_time_limit(0); //How long run? (seconds; 0==infinite)
$debug = false; //Use debuging mode? (Print errors and statistics)

//Filter
$eregi_url_blacklist = array(
    '(W3\.org|W3C)', //Hell knows...
    '(shop|xxx|porn|lesbian|hot)', //Commercial sites
    '(google|209.85.135.104|amazon.com|youtube.com)', //Big sites
    '(seznam.cz|centrum.cz|atlas.cz|zoznam.sk|quick.cz)', //Big local sites
    '.\.(css|ico|gif|jpg|png|bmp|cgi|js|vbs)', //Misc. webpage content
    '.\.(avi|mpg|mpeg|mov|wmv|wm|mp3|ogg|wma)', //Multimedia files
    '.\.(pdf|swf|flv|cfm)', //Other text files
    '.\.(exe|zip|rar|gz|bz|bz2|tar)' //Big and binary files
);

function check_crawl_url($url) { //Use this function to determine if url is wanted
    foreach($GLOBALS['eregi_url_blacklist'] as $black_url) {
        if(eregi($black_url, $url)) return(0);
    }
    if(in_array($url, $GLOBALS['urls'])) return(0);
    $file_url = explode('?', $url, 2); $file_url = $file_url[0];
    if(in_array($file_url, $GLOBALS['history'])) return(0);
    return(1);
}

///CODE
if($delete_url_db) @unlink($file);
if($debug) { $u = 0; $c = 0; $t = time(); }
$history[0] = '';
$fp = fopen($file, 'a+');
while(true) {
	if(sizeof($urls) <= 0) {
		$urls='';
		$urls[0]=$seed;
		if($buffer_increase) $max_urls++; //Increase size of buffer by one
		if($debug) echo("!Buffer underrun! !Buffer size is: $max_urls!\n");
	}
	
	$url = array_shift($urls);
	if($debug) echo("-Parsing: $url\n");
	preg_match_all('(http:\/\/[_a-zA-Z0-9\.\-]+\.[a-zA-Z]{2,4}\/{1}[-_~&=\ ?\.a-z0-9\/]*)',htmlspecialchars_decode(@file_get_contents($url, false, null, 0, $max_size)), $new_urls);
	$new_urls = $new_urls[0];
	
	foreach($new_urls as $new_url) {
		if($debug) $c++;///Debug
		if(check_crawl_url($new_url)) {
			echo($new_url."\n");	
			fwrite($fp, $new_url."\n");
			$file_url = explode('?', $new_url, 2); $file_url = $file_url[0];
			array_push($history, $file_url);
			while(sizeof($history) > $history_max) @array_shift($history);
			if($random_url) { if(sizeof($urls) < $max_urls) array_push($urls, $new_urls[rand(0,sizeof($new_urls)-1)]); }
				else { if(sizeof($urls) < $max_urls) array_push($urls, $new_url); }
			if($debug) { ///Debug Block
				$u++;
				$uspeed = round($u/(time()-$t+1), 2);
				$cspeed = round($c/(time()-$t+1), 2);
				$time = round((time()-$t)/60, 1);
				$buffered = sizeof($urls);
				$historied = sizeof($history);
				echo("+$u URLs; $c Downloaded; UpTime: $time mins; Buffered: $buffered URLs; History: $historied URLs; Speed: $uspeed URLs/s, $cspeed Downloads/s\n");
			}
		}
	}
	$new_urls = ''; //Memory cleanup
}

##EOF