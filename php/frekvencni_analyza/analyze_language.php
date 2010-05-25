#!/usr/bin/php
<?php
//Frequency analysis 0.2
//Harvie 2oo7
/*
 * Use this to crack monoalphabetic ciphers (vigenere,...)
 * It needs good language file (ex: one for technical another for medical language, etc...)
 * For more see this: http://en.wikipedia.org/wiki/Frequency_analysis_(cryptanalysis)
 *
 * Known bugs: this can assign one letter to more crypted letters
 */


function freq_analyze($infile, $cereg = '[a-z ]') { //Make frequency fingerprint
	$debug = 100;
	$total = 0;
	$in = fopen($infile, 'r');
	while(!feof($in)) {
		$c = strtolower(fgetc($in));
		if(eregi($cereg, $c)) {
			if(!isset($data[$c])) $data[$c] = 0;
			$data[$c]++;
			$total++;
		}
	}
	fclose($in);

	//Compute percents
	foreach($data as $c => $n) {
		$data[$c] = ($n/$total)*100;
		$debug -= $data[$c];
	}

	//echo("Debug: $debug\n"); //Debug
	return($data);
}

function freq_compare($cipher, $language) { //Do analysis (from two fingerprints)
	foreach($cipher as $c => $n) {
		$min = 1024;
		foreach($language as $dc => $dn) {
			$nmin = abs($n-$dn);
			//echo $nmin."\n"; //Debug
			if($nmin < $min) {
				$min = $nmin;
				$decrypted[$c] = $dc;
			}
		}
	}
	
	return($decrypted);
}

function freq_errors($decrypted) { //Test success (run with uncrypted cipher)
	$errors = 0;
	$total = 0;
	foreach ($decrypted as $a => $b) {
		if($a != $b) $errors++;
		$total++;
	}
	$hits = $total - $errors;
	$perc = round(($hits/$total)*100);
	echo("$hits hits and $errors errors / $total total - that means, you are $perc% successfull\n");
	return $errors;
}

///CODE//////////////////////////////////////////////////////////////////////////
$czech = freq_analyze('czech.txt');
//print_r($czech); //Debug
$cipher = freq_analyze('cipher.txt');
//print_r($cipher); //Debug

$decrypted = freq_compare($cipher, $czech);
freq_errors($decrypted);
print_r($decrypted);