<?php

function dir_m3u($dir) {
  $files=opendir ($dir); 
		while (false!==($file = readdir($files))) 
		{ 
				if ($file != "." && $file != "..") 
				{
          echo "$dir$file\">$file</a><br />\n"; 
          $num = ($num + 1);
				}
		}
		closedir($files); 
}

$dir = ".";
if(isset($_GET["dir"])) $dir = $_GET["dir"];

