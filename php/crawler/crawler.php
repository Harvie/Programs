#!/usr/bin/php
<?php
//HSp33der Web Crawler 5.6
//<-Harvie 2oo7/8
/*
 * Description:
 * This script crawls the web and printing found URLs to STDOUT.
 *
 * Installation:
 * - Debian:
 *   # apt-get install php5-cli php5-curl curl sort
 *   $ chmod +x crawler.php
 *   $ ./crawler.php (or $ php crawler.php)
 * - Windows:
 *   c:/>path/to/php5/php.exe crawler.php
 *   (you can add php.exe to PATH or associate .php files with it)
 *
 * Notes:
 * - You can use EtherApe (or similar sniffer) to get graphical
 * illustration of connecions (spider with your machine in center).
 * - There are few bugs in PHP5,
 * so this crawler may freeze after few minutes,
 * but you can use $use_curlbin option (need some extra files).
 * Check this for more info: http://bugs.php.net/bug.php?id=43098
 *
 * Tips & Tricks:
 * - Make found URLs 100% unique:
 * cat urls.txt | sort -u > uurls.txt && mv -f uurls.txt urls.txt
 * - Show filesize and total urls count:
 * du -h urls.txt && cat urls.txt | wc -l
 * - Open another output pipe:
 * tail -f urls.txt
 * - Use some plugin (filter) to extract emails, etc...
 * tail -f urls.txt | ./example_mail_extractor.php
 *
 * Special thx2:
 * - PHP, cURL (technology)
 * - root.cz & php.net (help)
 * - Linus Torvalds (because I am crawling from Linux)
 */

///SETTINGS//////////////////
//Basic
$seed = 'http://blog.harvie.cz/'; //Start crawling from this page
$file = 'urls.txt'; //File to save crawled URLs (use sort to make URLs 100% exclusive)
$delete_url_db = false; //Delete file with saved URLs before crawling
$restore = true; //Use last url from $file instead of seed
//Advanced
$max_size = 10000; //How many bytes download from each page?
$max_urls = 30; //Size of URL buffer (prebuffered URLs to crawl, if full, no more URLs will be added to queue)
$buffer_increase = true; //Increase buffer at buffer underrun?
$random_url = false; //Select random urls from page? (This may override URL filter)
$history_max = 1000; //How many last URLs keep in history? (historied URLs will not be crawled again...)
//Curl binary plugin
$use_curlbin = true; //Use interface to cURL executable instead of file_get_contents() (More speed, less crashes)
$curlpath = 'curl'; //Path to cURL executable binary (you can download it from http://curl.haxx.se/)
//Sort binary plugin
$use_sortbin = true; //Use sort to make URLs unique?
$sortpath = 'sort'; //Path to SORT executable binary (get it from your OS or UNXUtils for Windows)
$sort_each = $history_max; //Sort file after X new URLs
//System/PHP
$socket_timeout = 1; //How long you want wait for a webserver? (seconds)
$time_limit = 0; //How long run? (seconds; 0==infinite)
$memory_limit = '128M'; //Maximum memory to eat (if exhausted, crawler will stop)
//Debug
$debug = true; //Use debuging mode? (Print errors and statistics)

//Filter
$eregi_url_blacklist = array( //Most important thing when crawling ;D
    '(W3\.org|W3C)', //Hell knows...
    '(shop|xxx|porn|lesbian|hot)', //Commercial sites
    '(google|209.85.135.104|yahoo.com|amazon.com|youtube.com)', //Big sites
    '(seznam.cz|centrum.cz|atlas.cz|zoznam.sk|quick.cz)', //Big local sites
    '.\.(css|ico|gif|jpg|png|bmp|cgi|js|vbs)', //Misc. webpage content
    '.\.(avi|mpg|mpeg|mov|wmv|wm|mp3|ogg|wma)', //Multimedia files
    '.\.(pdf|swf|flv|cfm)', //Other text files
    '.\.(exe|zip|rar|gz|bz|bz2|tar)' //Big and binary files
);


///FUNCTIONS/////////////////
function shell_sort_file($file, $tmpfile='.tmp') {
    $tmpfile = $file.$tmpfile;
    if($GLOBALS['debug']) echo("-SORTing $file (using tempfile $tmpfile)\n");
    @unlink($tmpfile);
    system($GLOBALS['sortpath']." -u $file > $tmpfile");
    if(is_file($tmpfile) && (filesize($tmpfile) > 3)) {
        unlink($file);
        rename($tmpfile, $file);
    } else {
        if($GLOBALS['debug']) echo("-SORTing failed!!!\n");
    }
     if($GLOBALS['debug']) echo("-SORTing done!\n");    
}

function shell_curl_get($url, $timeout=0, $maxdata=0) { //I made this cURL interface cause file_get_contents() absolutely sux
    $curl = 'curl';
    if(isset($GLOBALS['curlpath'])) $curl = $GLOBALS['curlpath'];
    $args = "--get --location --connect-timeout $timeout --max-time $timeout --compressed --stderr curl.debug";
    $url = escapeshellcmd(trim($url));
    $cmd = "$curl --url \"$url\" $args";
    if($GLOBALS['debug']) echo("-Ex: $cmd\n");
    $fp = popen($cmd, 'r');
    stream_set_timeout($fp, $timeout);
    stream_set_blocking($fp, 0);
    //if($timeout>0) sleep($timeout);
    $data = '';
    while(!feof($fp)) {
        $in = fgets($fp, 100);
        $data .= $in;
        if($maxdata>0 && strlen($data)>=$maxdata) break;
        if($in == '') usleep(1000000/8);
    }
    @fclose($fp);
    return $data;
}

function check_crawl_url($url) { //Use this function to determine if url is wanted
    foreach($GLOBALS['eregi_url_blacklist'] as $black_url) {
        if(eregi($black_url, $url)) return(0);
    }
    if(in_array($url, $GLOBALS['urls'])) return(0);
    $file_url = explode('?', $url, 2); $file_url = $file_url[0];
    if(in_array($file_url, $GLOBALS['history'])) return(0);
    return(1);
}


///CODE//////////////////////
//Init
if($restore && is_file($file)) { //Load last url as seed
    $urls[1]=$seed; //original seed as backup (if no urls found at restored page)
    if($debug) echo("!Restoring: ");
    $rest = fopen($file, 'r');
    while(!feof($rest)) {
        $seed = $new;
        $new = fgets($rest);
    }
    fclose($rest);
    $seed = trim($seed);
    if($debug) echo($seed."\n");
    $urls[0]=$seed;
    $seed=$urls[1];
}

if($delete_url_db) @unlink($file);
ini_set('default_socket_timeout',$socket_timeout);
set_time_limit($time_limit);
ini_set('memory_limit', $memory_limit);

//Loop
if($debug) { $u = 0; $c = 0; $t = time(); }
$history[0] = '';
$sort_next = $sort_each;
$sorted_db = 0;
$fp = fopen($file, 'a+');
while(true) { //MAIN L00P
    if(sizeof($urls) <= 0) {
        $urls='';
        $urls[0]=$seed;
        if($buffer_increase) $max_urls++; //Increase size of buffer by one
        if($debug) echo("!Buffer underrun! !Buffer size is: $max_urls!\n");
    }
    
    //Get URL from buffer, download few bytes and write down URLs to array:
    $url = array_shift($urls);
    if($debug) echo("-Parsing: $url\n");
    if(!$use_curlbin) { $page = @file_get_contents($url, false, null, 0, $max_size); }
    else { $page = shell_curl_get($url, $socket_timeout, $max_size); }
    preg_match_all('(http:\/\/[_a-zA-Z0-9\.\-]+\.[a-zA-Z]{2,4}\/{1}[-_~&=\ ?\.a-z0-9\/]*)',htmlspecialchars_decode($page), $new_urls);
    $new_urls = $new_urls[0];
    
    foreach($new_urls as $new_url) { //Process grabed URLs
        if($debug) $c++;///Debug
        if(check_crawl_url($new_url)) {
            echo($new_url."\n");    
            fwrite($fp, $new_url."\n");

            if($use_sortbin) { //Handle sorting (unique URLs)
                $sort_next--;
                if($sort_next<=0) {
                  fclose($fp);
                    shell_sort_file($file);
                    if($debug) {
                        $sorted_db = 0;
                        $fp = fopen($file, 'r');
                        while(!feof($fp)) {
                            fgets($fp);
                            $sorted_db++;
                        }
                        fclose($fp);
                    }
                    $fp = fopen($file, 'a+');
                    $sort_next = $sort_each;
                }
            }

            $file_url = explode('?', $new_url, 2); $file_url = $file_url[0];
            array_push($history, $file_url);
            while(sizeof($history) > $history_max) @array_shift($history);
            if($random_url) { if(sizeof($urls) < $max_urls) array_push($urls, $new_urls[rand(0,sizeof($new_urls)-1)]); }
                else { if(sizeof($urls) < $max_urls) array_push($urls, $new_url); }
            if($debug) { ///Debug Block
                $u++;
                $sorted_db++;
                $uspeed = round($u/(time()-$t+1), 2);
                $cspeed = round($c/(time()-$t+1), 2);
                $time = round((time()-$t)/60, 1);
                $buffered = sizeof($urls);
                $historied = sizeof($history);
                $memory = round(memory_get_usage()/1000000, 2);
                echo("+$u ($sorted_db sorted) URLs; $c Downloaded; UpTime: $time mins; Buffered: $buffered URLs; History: $historied URLs; To sort: $sort_next URLs; Speed: $uspeed URLs/s, $cspeed Downloads/s; Memory used: $memory/$memory_limit\n");
            }
        }
    }
    $new_urls = ''; //Memory cleanup
}

##EOF
