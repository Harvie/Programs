<?php
//Lyrics bot 0.1
//<-Harvie 2oo7
/*
This will search and show lyrics of specified song from lyricsplugin.com...
If you need WinAmp or WMP plug-in, look here: http://www.lyricsplugin.com/
*/
///FUNCTIONS////////////////////////////////////////////////////////////////
function get_lyrics($artist, $title) { //Finds and returns lyrics of song
  $title = urlencode($title);
  $artist = urlencode($artist);
  $url = "http://www.lyricsplugin.com/plugin/?artist=$artist&title=$title";
  //readfile($url); //Debug
  if( !($fp = fopen($url, "r")) ) return("Error_1: Can't connect!");
  
  $lyrics = "";
  while( ($line = trim(fgets($fp))) != "<div id=\"lyrics\">" );
  while( ($line = trim(fgets($fp))) != "</div>" ) {
    $line = strip_tags($line);
    //echo($line."\n");
    $lyrics = $lyrics.$line."\n";
  }
  
  $lyrics = trim($lyrics);
  if($lyrics == "") $lyrics = "Error_2: Lyrics not found!";
  return($lyrics."\n");
}

function parse_music_info($filename) { //This returns array with music info (0 => Artist, 1 => Title) parsed from filename
  $filename = explode("-", $filename);
  $ret[0] = $filename[0];
  $filename = split("(-|\\.)", $filename[1]);
  $ret[1] = $filename[0];
  
  $ret = str_replace("_", " ", $ret);
  $ret[0] = trim($ret[0]);
  $ret[1] = trim($ret[1]);
  //print_r($filename); //Debug
  //print_r($ret); //Debug
  return $ret;
}

///CODE/////////////////////////////////////////////////////////////////////
$song = parse_music_info("Jimi_Hendrix_-_Are_you_experienced.mp3");
echo("Lyrics for: ".$song[1]." by: ".$song[0]."\n\n");
echo get_lyrics($song[0] ,$song[1])."\n";
//echo get_lyrics("Jimi Hendrix" ,"Are you experienced");

?>
