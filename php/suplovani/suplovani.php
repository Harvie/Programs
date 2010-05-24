#!/usr/bin/php
<?php
	$url = 'http://www.ssps.cz/pages/rozvrhy/index.php';
	$croom = '3.A';
	
	function rss_add_item($title, $link='', $desc='') {
		echo("\n<item>\n");
		  echo("\t<title>$title</title>\n");
		  echo("\t<link>$link</link>\n");
		  echo("\t<description>$desc</description>\n");
		echo("</item>\n\n");
	}
?>
<?php echo('<?xml'); ?> version="1.0" encoding="windows-1250"?><rss version="0.91"><channel>

<title>Zmeny v rozvrhu SSPS</title>
<link><?php echo($url); ?></link>
<description>Aktualni zmeny v rozvrhu SSPS</description>
<language>cs</language>

<?php
	$croom = str_replace('.', '\.', $croom);
	$feed = spliti('<.?..>', file_get_contents($url));

	foreach($feed as $line) {
		$line = trim($line);
		if($line == '' || eregi('rozvrhy', $line)) continue;
		if(eregi($croom, $line)) rss_add_item($line, $url);
	}
	
?>
			   
</channel></rss>
