<pre>
<h1>Torrent Search Bot</h1>
<form action="?" method="GET">
	<input type="text" name="q"><input type="submit" value="!SearchNow!">
</form>

<?php
set_time_limit(0); 

if(!isset($_GET['q'])) die('</pre>');
$q = $_GET['q'];

$q = urlencode($q);
$q = "http://www.google.com/search?num=100&q=$q%20filetype%3Atorrent";
echo("<a href=\"$q\">$q</a>\n\n");

preg_match_all('(http:\/\/[_a-zA-Z0-9\.\-]+\.[a-zA-Z]{2,4}\/{1}[-_~&=\ ?\.a-z0-9\/]*)',htmlspecialchars_decode(@file_get_contents($q)), $torrents);
//print_r($torrents);
$torrents = $torrents[0];
foreach($torrents as $torrent) if(eregi('\.torrent', $torrent)){
	echo("<a href=\"$torrent\">$torrent</a>\n");
}
?>
</pre>
