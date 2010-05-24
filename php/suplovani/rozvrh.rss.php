<?php

//URL Stránky s rozvrhy
$url = 'http://www.ssps.cz/pages/rozvrhy/index.php';

$fp = fopen('http://www.ssps.cz/pages/rozvrhy/index.php', 'r');
while(!feof($fp)) {
	$line = fgets($fp);
	//echo $line;
	if(eregi('Aktualizováno', $line)) {
		$line = str_replace('</DIV>', '', $line);
		$line = str_replace('&nbsp;', ' ', $line);
		$line = trim($line);
		//echo("<!-- $line -->\n");
		break;
	}
}

echo('<?xml version="1.0" encoding="ISO-8859-2"?>');
?>
<rss version="0.91"><channel>

<title>Zmeny v rozvrhu SSPS</title>
<link>http://www.ssps.cz/pages/rozvrhy/index.php</link>
<description>Aktualni zmeny v rozvrhu SSPS</description>
<language>cs</language>

<item>
	<title><?php echo($line); ?></title>
	<link>http://www.ssps.cz/pages/rozvrhy/index.php</link>
	<description><?php echo($line); ?></description>
</item>
   
</channel></rss>


