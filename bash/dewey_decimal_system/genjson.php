#!/usr/bin/php
<?php
//You will need JSON PHP extension for this script...

function recursive_scandir($path,&$dir) {
	foreach($dir as $key => $value) {
		$dir[$key] = array_flip(scandir($path.'/'.$key, 0));
		unset($dir[$key]['.']);	unset($dir[$key]['..']);
		recursive_scandir($path.'/'.$key,$dir[$key]);
	}
}

$dds = array_flip(array('dds'));
recursive_scandir('./', $dds);

//print_r($dds); //you can use this if you don't have JSON ext.
echo json_encode($dds);
