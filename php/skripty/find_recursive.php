<pre>
<?php

//
function dir_scan($directory, $slash="/") {

	if($GLOBALS["num"] == 0) echo("<title>$directory</title>\n<h1><a href=\"file:///$directory\">$directory</a></h1>\n");
	$files=opendir ($directory); 
		while (false!==($file = readdir($files))) 
		{ 
				if ($file != "." && $file != "..") 
				{ 
						if(is_dir($directory.$file)) echo("[DIR] ");
						//echo "$directory$file\n"; 
						echo "<a href=\"file:///$directory$file\">$directory$file</a>\n"; 
            $GLOBALS["num"] = ($GLOBALS["num"] + 1);
						if(is_dir($directory.$file)) dir_scan($directory.$file.$slash,$slash);
				} 
		}
		closedir($files); 

}

///CODE////////////////////////
$num = "0";
dir_scan("x:\\", "\\");
echo("\nTotal: $num\n");
?>
</pre>
