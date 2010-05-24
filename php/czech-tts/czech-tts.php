#!/usr/bin/php
<?php
/* Sources:
	http://en.wikiversity.org/wiki/Czech_Pronunciation
	http://victorian.fortunecity.com/vermeer/287/czechtable.htm
	http://www.google.cz/search?q=czech+pronunciation
*/

$text = 'žluťoučký kůň příšerně úpěl ďábelské ódy.';
$text = 'dobrý den!';

$trans = array(
	"au" => "ou",
	"ou" => "ew",
	"ch" => "cch",

	//ertuioasdycnz
	"á" => "aa",
	"č" => "ch",
	"ď" => "d", //
	"é" => "ea",
	"ě" => "ie",
	//"ch" => "",
	"í" => "ee",
	//"ň" => "",
	"ó" => "oo",
	"ř" => "rsch", //
	"š" => "sch",
	"ť" => "t", //
	"ú" => "oo",
	"ů" => "oo",
	"ý" => "ee",
	"ž" => "ge",
	//"" => "",

	"0" => "0"
);

$text = strtr($text, $trans);

echo($text."\n");
system('espeak ' . escapeshellarg($text));

?>
