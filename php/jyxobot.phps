<?php
//Jyxo seek
//<-Harvie (2oo7)
/*
Tento skript vam umozni vyhledavat a automaticky stahovat
multimedialni obsah parsovanim z vyhledavaciho enginu jyxo.cz.
*/

function url2filename($url) {
  $url = explode("/", $url);
  return $url[(sizeof($url)-1)];
}
//die(url2filename("http://johnys-pages.wbs.cz/03_-_Bob_Marley_-_No_woman_no_cry.mp3")); //Debug

function jyxo_bot($q="",$d="mm",$ereg=".",$notereg="",$cnt=10000000000000,$page=1,$pmax=2,$o="nocls") {

  $i = 0;
  $results = "";
  $results[$i] = "";
  //$q = str_replace(" ", "+", $q);
  $q = urlencode($q);
  for(;$page<=$pmax;$page++) {
    $request = ("http://jyxo.cz/s?q=$q&d=$d&o=$o&cnt=$cnt&page=$page");
    $fp = fopen($request, "r") or die("   !!! Cannot connect !!!");
    while(!feof($fp)) {
      $line = fgets($fp);
      if(eregi("<div class='r'>", $line) && ereg(" class=ri", $line)) {
        $line = explode("<!--m--><div class='r'><A HREF=\"", $line);
        $line = $line[1];
        $line = explode("\" class=ri", $line);
        $line = trim($line[0]);
        $line = urldecode($line);
      
        if(@eregi($ereg, $line) && !@eregi($notereg, $line) && !in_array($line, $results)) {
          echo("$line\n"); //Output
          //echo("$i:$line\n"); //Indexed Output
          //echo("<a href=\"$line\">$line</a><br />\n"); //XHTML Output
          $results[$i] = $line;
          $i++;
        }
      
      }
    }
    fclose($fp);
  }
  echo("\nTotal: $i\n"); //Sumary Output
  return $results;
}

//
function download_url_array($results, $download_dir) {
  //$download_dir = $GLOBALS["download_dir"];
  $fails = 0; $done = 0;
  echo("\nDownloading to $download_dir ...\n");
  if(!is_dir($download_dir)) {
    echo("Creating directory: $download_dir\n\n");
    mkdir($download_dir);
  } else { echo("\n"); }
  foreach($results as $id => $url) {
    $file = url2filename($url);
    echo("Downloading $file (#$id)... ");
    $file = "$download_dir\\$file";
    if(!is_file($file) && @copy($url, $file)) {
      echo("Done.\n");
      $done++;
    } else {
      if(is_file($file)) { echo("File already exists "); }
      echo("Failed!\n");
      $fails++;
    }
  }
  $totaldls = ($fails+$done);
  echo("\n$done/$totaldls files successfully downloaded to $download_dir ($fails failed)\n");
}

///CODE//////////////////////////////////////////////////////////////////////////////////////
$results = jyxo_bot("interpret titul","mm","(.mp3|.ogg)"); //Text2Search
//print_r($results); //Array Output
//var_dump($results); //Detail debug output
//download_url_array($results, "X:\\JyxoBot"); //Automatic Download

?>
