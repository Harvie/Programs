<?php
//Jyxo seek
//<-Harvie (2oo7)

function url_bot($url) {
  $file = file($url);
  $file = implode("\n", $file);
  $file = spliti("http://", $file);
  //print_r($file);
  $urls[-1] = "";
  $i = 0;
  foreach($file as $url) {
    $url = split(">| ", $url);
    //print_r($url);
    $url = $url[0];
    if(eregi(".com", $url)) {
      $urls[$i] = "http://".$url;
      $i++;
    }
  }
  //print_r($urls);
  return($urls);
}

function wmv_bot($url, $ext) {
  foreach(url_bot($url) as $url) {
    $file = file($url);
    $file[-1] = "";
  $file = implode("\n", $file);
  $file = spliti("http://", $file);
  //print_r($file);
  $urlz[-1] = "";
  $i = 0;
  foreach($file as $url) {
    $url = explode(">", $url);
    $url = $url[0];
    if(eregi(".com", $url) && eregi($ext, $url) && !eregi(" |\"", $url)) {
      $urlz[$i] = "http://".$url;
      $i++;
    }
  }
  print_r($urlz);
  }
  return($urlz);
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
$results = wmv_bot("http://www.loliti.com/cz/m/bonde19e_s_vodou_0.php", ".wmv");
print_r($results);
//download_url_array($results, "X:\\wmvBot"); //Automatic Download

?>
