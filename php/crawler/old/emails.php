#!/usr/bin/php
<?php
// PHP Version => 5.2.0-8+etch7 (ihrisko.org)



//phpinfo(); die;

ini_set('default_socket_timeout',1); //How long wait for a webserver? (seconds)
set_time_limit(0); //How long run? (seconds)
//$url='http://ad.doubleclick.net/click';
$url='http://w.moreover.com/';
$max_size = 10000;
$i = 0;
while(1) {
file_get_contents($url, false, null, 0, $max_size);
//preg_match_all('(http:\/\/[_a-zA-Z0-9\.\-]+\.[a-zA-Z]{2,4}\/{1}[-_~&=\ ?\.a-z0-9\/]*)',htmlspecialchars_decode(@file_get_contents($url, false, null, 0, $max_size)), $new_urls);
echo "#";
$new_urls = $new_urls[0];
$i++;
print_r($new_urls); echo($i);
}
die;

$stdin = fopen('php://stdin','r');
while(!feof($stdin)) {
	$url = fgets($stdin);
	preg_match_all("/[-a-z0-9\._]+@[-a-z0-9\._]+\.[a-z]{2,4}/", @file_get_contents($url), $emails); $emails = $emails[0];
	foreach($emails as $email) {
		echo($email."\n");
	}
}


die;

?>
Hi everybody!
I am trying to write WebCrawler/Spider (as school project, and -of course- I am trying to be more wealthy than google ;o)

So... I have big/small problem:
I am using file_get_contents() (i've tryed fopen() too...).
Crawler works 100% great, but sometimes it freezing. I have tryed to trace what function freezes, and i found it, it's file_get_contents()...

So, i googled and found default_socket_timeout setting, i set it to 1, but sometimes its freezes and never get up again.

I've done this example, so you can see, that it freezes after few iterations. I have supplyed URL, that causes freeze of my crawler (im not sure why...):

#!/usr/bin/php
< ?php

ini_set('default_socket_timeout',1);
set_time_limit(0);
//$url='http://ad.doubleclick.net/click';
$url='http://w.moreover.com/';
while(1) {
    @file_get_contents($url, false, null, 0, 10000);
    echo "#";
}

?>

Of course, if somebody want be better than google, he have to have very good crawler. So I need very solid code, that can run and crawl a days without crash (like this one). Yeah, it's true, that this worked 1 or 2 hours before it crashes, or i stoped it, but the file_get_contents() doesn't work like i need.

If you are interested to crawling, you can write me to YouSeekMe: 283//782//978 ;D

And there are few statistics from my last session:
+5431 URLs; 19292 Downloaded; UpTime: 21.5 mins; Buffered: 30 URLs; History: 1000 URLs; Speed: 4.22 URLs/s, 14.98 Downloads/s

THX4AnyHelp ;o)