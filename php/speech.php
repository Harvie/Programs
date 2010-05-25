<title>Harvie's Remote TTS</title>
<link rel="stylesheet" type="text/css" href="admin.css">

<form action="?" method="GET">
	<input type="text" name="tts">
	<input type="password" name="pwd" value="<?php echo($_GET['pwd']); ?>">
	<input type="submit" value="ODESLAT!">
</form>

<?php
$passwd='posli';
if($_GET['pwd'] != $passwd) die('<br />Wrong password!');
$text=$_GET['tts'];
echo("?tts=$text");

function festival_say($text, $encoding='UTF-8') {
        //Maybe you will need to SUID festival binary...
	$festival = popen("iconv -c -t ISO-8859-2 -f $encoding | festival --tts", 'w');
	fwrite($festival, $text);
	fclose($festival);
}

festival_say($text);




