#!/usr/bin/php
<?php

function obcanka_ocr_line($country, $name, $familyn, $sex, $opno, $personalno, $expires_YYMMDD) {
	$experno = explode("/", $personalno);
	$birthnum = $experno[0].$experno[2];
	$birthnum[2] = 0;

	$blankline = "####################################";	

	$nameline = "ID$country$familyn<<$name$blankline";
	$nameline = str_replace("#", "<", $nameline);
	$nameline = substr($nameline, 0, 36);
	//echo strlen($nameline)."\n";
	echo("$nameline\n");

	$numline = $opno."1".$country.$birthnum."1".$sex.$expires_YYMMDD.$experno[1]."<<<".rand(0,9);
	echo("$numline\n");
}

obcanka_ocr_line("CZE", "TOMAS", "NOVAK", "M", "204672478", "800456/0096", "100202");
