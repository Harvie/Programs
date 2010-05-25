<?php
	header("Content-Type: application/atom+xml");

        $url = 'http://www.ssps.cz/pages/rozvrhy/index.php';

        # E.g. $croom = '1.A';
	$croom = "x";
        $croom = trim($_GET['q']);

        function atomPrintItem($title, $id, $update, $link, $desc='') {
                echo("\n<entry>\n");
		echo("\t<id>$id</id>\n");
		echo("\t<updated>" . strftime("%Y-%m-%dT%H:%M:%SZ", $update) . "</updated>\n");
                echo("\t<title>$title</title>\n");
                echo("\t<link href=\"$link\" />\n");
                echo("\t<content>$desc</content>\n");
                echo("</entry>\n\n");
        }
	echo('<?xml'); ?> version="1.0" encoding="UTF-8"?>
<feed xmlns="http://www.w3.org/2005/Atom" xml:lang="cs">
	<title>Změny v rozvrhu SSPŠ</title>
	<subtitle>Aktuální změny v rozvrhu SSPŠ</subtitle>
	<link href="<?php echo($url); ?>" />
	<icon>http://www.ssps.cz/favicon.ico</icon>
	<id>tag:ssps.cz,2008:</id>
	<author>
		<name>SSPŠ</name>
	</author>
<?php
        $croom = str_replace('.', '\.', $croom);
        $feed = split('<[^>]+>', iconv('WINDOWS-1250', 'UTF-8', file_get_contents($url)));
	$date = 0;
	$update = 0;
	$lastUpdate = 0;
 
	ob_start();

        foreach($feed as $line) {
                $line = trim($line);
                if($line == '' || eregi('rozvrhy', $line)) continue;

		# Last update?
		if (eregi("^Aktualizováno: (..)\.(..)\.(....)&nbsp;(..:..:..)$", $line, $r))
		{
			$update = strtotime("{$r[3]}-{$r[2]}-{$r[1]} {$r[4]}");

			# This is the last update of the whole feed
			if ($lastUpdate < $update)
				$lastUpdate = $update;
			continue;
		}

		# What day are we reading?
		if (eregi("^[^ ]+&nbsp;(..)\.(..)\.(....)$", $line, $r))
		{
			$date = strtotime("{$r[3]}-{$r[2]}-{$r[1]}");
			continue;
		}

		# Found a line about our class
		if ($date && eregi($croom, $line))
		{
			$days = array('Ne', 'Po', 'Út', 'St', 'Čt', 'Pá', 'So');
			$wday = localtime($date, TRUE);
			$wday = $days[$wday['tm_wday']];
			atomPrintItem($wday . strftime(" %d.%m. ", $date) . $line,
				"tag:ssps.cz," . strftime("%Y-%m-%d:", $date) . md5($line),
				$update, $url, $line);
		}
        }
	$feed = ob_get_contents();
	ob_end_clean();
	# Must appear before the items
	echo ("\t<updated>" . strftime("%Y-%m-%dT%H:%M:%SZ", $lastUpdate) . "</updated>\n");
	echo $feed;
?>
</feed>

