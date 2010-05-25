<?php

//Rekurzivní výpis souborù a adresáøù v podadresáøích
//Demonstrace rekurzivní funkce
//Author: <-Harvie 2oo7

function dir_tree($directory, $level) {

  //$num = "1";
  
  //echo ("<b>Soubory v $directory :</b><br /><br />\n<ul>");

	$dirfp = opendir ($directory); 
  $leveli = $level;
              while($leveli) { echo("-"); $leveli--; }
  echo "[DIR] $directory\n";
		while (false!==($file = readdir($dirfp))) 
		{ 
				if ($file != "." && $file != "..") 
				{ 
            if (is_dir($directory.$file))
            {
              echo("\n");
              $leveli = $level;
              while($leveli < 0) { echo("-"); $leveli--; }
              //echo "-[DIR] $file\n";
              dir_tree("$directory$file\\", ($level+2));
              echo("\n");
            }
            else
            {
              $leveli = $level;
              while($leveli + 1) { echo("-"); $leveli--; }
              echo "$file - ($directory$file)\n"; 
              //$num = ($num + 1);
            }
				} 
		}
		closedir($dirfp); 

}

//CODE

$dir = ("D:\\Webdesign\\");
echo("FreeSpace: ".(disk_free_space($dir)/1000000)." MB /".(disk_total_space($dir)/1000000)." MB\n");
dir_tree($dir, 0);

?>
