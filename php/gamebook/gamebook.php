<html><?php
///////////////////////
//GameBook Engine 0.1
//Coded: Harvie 2oo7
///////////////////////

//Config:
$osud = "Osud rozhodl: ";

//PARAMETERS///////////////////////////////////////////////////////
if (isset($_GET["book"]))
	{ $book = $_GET["book"]; }
	else
	{ $book = "game01.txt"; }

if (isset($_GET["pos"]))
	{ $pos = $_GET["pos"]; }
	else
	{ $pos = 2; }

///////////////////////////////////////////////////////////////////

//Init:
$game = file($book);
$descs = explode("|", $game[0]);
//print_r($descs);
$room = $game[($pos - 1)];


?>
<head>
	<title><?php echo($descs[0]); ?></title>
</head>

<body>
<h1><?php echo($descs[0]); ?> ( OpenGameBook )</h1>
<h2><?php echo($descs[1]); ?></h2>
<h3>Kapitola: <?php echo($descs[2]); ?></h3>
<?php echo($descs[3]); ?>
<hr />
<a href="?book=game01.txt">?book=game01.txt</a> - UKAZKOVA HRA<br />
<a href="?book=game01.txt&pos=1">?book=game01.txt&amp;pos=1</a> - UVOD<br />
<a href="?book=game01.txt&pos=2">?book=game01.txt&amp;pos=2</a> - UKAZKOVY SAVEGAME<br />
<a href="./game01.txt">./game01.txt</a> - UKAZKA DATOVEHO SOUBORU GAMEBOOKU
<hr /><br /><br />
<?php


//Random:
if ( strpos( (" ".$room), "{") != 0 ) {
	$rnd = explode("{", $room);
	$rnd = explode("}", $rnd[1]);
	$rnd = explode("\\\\", $rnd[0]);
	$irand = rand(0, (sizeof($rnd)-1) );
	$osudout = ("<br /><a href=\"?book=".$book."&pos=".$rnd[$irand]."\">".$osud . $rnd[$irand]."</a>"); 
}

//Parse links:
$room = str_replace("[", "<a href=\"?book=".$book."&pos=", $room);
$room = str_replace("|", "\">", $room);
$room = str_replace("]", "</a>", $room);
$room = str_replace("\\\\", " nebo ", $room);

//Output:
echo ("<b>".$pos.":</b> ".$room);
echo ($osudout);

?>
<hr />
</body>

</html>