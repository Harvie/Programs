<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<html>
<head>
<meta http-equiv="content-type" content="text/html"; charset="utf-8">
<title>Kniha Návštěv - (c) 2oo6 - Harvie</title>
<meta http-equiv="cache-control" content="no-cache" />

<style>

body {
	background-color: skyblue;
	text-align: left;
	padding: 4px 10% 4px 10%;
	font-family: Tahoma;
	color: black;
	font-size: small;
	}

.form {
	width: 80%;
	background-color: lightblue;
	border: 2px;
	border-style: solid;
	padding: 10px;
	margin: 4px 10% 4px 10%;
	}
	
.form input, textarea {
	border: 1px;
	border-style: solid;
	border-color: black;
	width: 100%
	}
	
.prispevek {
	width: 80%;
	background-color: lightblue;
	border: 2px;
	border-style: solid;
	padding: 10px;
	margin: 4px 10% 4px 10%;
	}
	
.prispevek .nadpis {
	padding: 2px;
	font-weight: bold;
	text-decoration: none;
	color: black;
	font-size: small;
	border: solid skyblue 2px;
	border-bottom: none;
	_font-size: x-small;
	}
	
.prispevek .jmeno {
	padding: 2px;
	font-weight: bold;
	font-style: italic;
	text-decoration: none;
	color: darkblue;
	font-size: small;
	border: solid skyblue 2px;
	border-bottom: none;
	border-top: none;
	_font-size: x-small;
	}
	
	.prispevek .datum {
	padding: 2px;
	text-decoration: none;
	color: darkred;
	border: solid skyblue 2px;
	border-top: none;
	font-size: x-small;
	}
	
.prispevek .text {
	padding: 2px;
	font-size: small;
	border: dotted skyblue 2px;
	background-color: skyblue;
	overflow: hidden;
	_font-size: x-small;
	}

</style>

</head>

<body>

<div class="form">
<form action="" method="post">
		<h2>Kniha návštěv</h2>

		<p>
			<label for="title"><b>Nadpis:</b><br /></label>
			<input type="text" name="title" value="" />
		</p>

		<p>
			<label for="name"><b>Jméno:</b><br /></label>
			<input type="text" name="name" value="" />
		</p>

		<p>
			<label for="contact"><b>Kontakt:</b><br /></label>
			<input type="text" name="contact" value="EMAIL:      ICQ:      WWW:      TEL/FAX:    " />
		</p>

		<p>
			<b>Text příspěvku:</b><br />
			<textarea rows="10" cols="30" name="text" title="podporované tagy: [b][/b] [i][/i] [u][/u] - Příliš dlouhá slova budou zkrácena."></textarea>
		</p>

		<p>
			<label for="sprot"><b title="Brání knihu proti náhodnému napadení spamboty.">Ochrana proti spamu:</b> <font title="sedm - numericky">[(4)+(3)]=</label>
			<input type="text" name="sprot" value="" />
		</p>
		
		<input type="hidden" name="sent" value="true" />

		<p>
			<input type="submit" value="Uložit příspěvek" />
		</p>
		
	</form>

	<form action="delete.php">
	<input type="submit" value="Smazat knihu">
	</form>

<?php

if (($_POST["title"]!='') && ($_POST["name"]!='') && ($_POST["text"]!='') && ($_POST["sprot"]=='7')):

	//Načtení z POSTu; Převedení tagů na entity; Ořezání bílých znaků na začátku a konci;

	$title = $_POST["title"];
	$title = HTMLSpecialChars($title);
	$title = trim($title);

	$name = $_POST["name"];
	$name = HTMLSpecialChars($name);
	$name = trim($name);

	$contact = $_POST["contact"];
	$contact = HTMLSpecialChars($contatc);
	$contatct = trim($contact);

		//Parsování tagů v hlavním textu zprávy
	$text = $_POST["text"];
	$text = HTMLSpecialChars($text);

		//Definice vlastních tagů
		/*
			[b]ahoj[/b]
			[i]ahoj[/i]
			[u]ahoj[/u]
			[tt]ahoj[/tt]
			[pre]ahoj[/pre]
		*/

		$text = str_replace("[b]","<b>", $text);
		$text = str_replace("[/b]","</b>", $text);

		$text = str_replace("[i]","<i>", $text);
		$text = str_replace("[/i]","</i>", $text);

		$text = str_replace("[u]","<u>", $text);
		$text = str_replace("[/u]","</u>", $text);

		$text = str_replace("[tt]","<tt>", $text);
		$text = str_replace("[/tt]","</tt>", $text);

		$text = str_replace("[pre]","<pre>", $text);
		$text = str_replace("[/pre]","</pre>", $text);

$text = nl2br($text);

	//Zjištění data a času
	//SetLocale("LC_ALL", "Czech"); //Nastavení českého prostředí
	$datum = Date("j/m/Y H:i:s", Time());

	//Otevření souboru
	$fp = FOpen ("gbook.txt", "a+");

	//Zápis dat do souboru
	FWrite ($fp, "<div class=\"prispevek\">\n");

	FWrite ($fp, "<meta name=\"IP\" content=\"" . $_SERVER[REMOTE_ADDR] . "\">\n");

	FWrite ($fp, "<div class=\"nadpis\">".$title."</div>\n");
	FWrite ($fp, "<div class=\"jmeno\"> vložil: ".$name." - ".$contact."</div>\n");
	FWrite ($fp, "<div class=\"datum\">".$datum."</div><br />\n");

	FWrite ($fp, "<div class=\"text\">\n");
	FWrite ($fp, $text);
	FWrite ($fp, "\n</div></div>\n\n");

	//Uzavření souboru
	FClose ($fp);

	//Výpisy
	echo "<b><font color=\"red\">Příspěvek byl korektně uložen!<br />Vaše IP adresa: " . $_SERVER[REMOTE_ADDR] . " byla uložena!</font></b> <meta http-equiv=\"refresh\" content=\"1\">";

else:

	echo "<b><font color=\"red\" title=\"Nezapomeňte na pole ochrany proti spamu!!!\">Nezapomeňte vyplnit všechna pole!</font></b>";

endif;

?>
</div>

<?php
//Vypsání celého souboru
$fp = FOpen ("gbook.txt", "r+"); 
FPassThru($fp);
FClose ($fp);
?>

</body>
</html>
