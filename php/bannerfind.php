<?php
//Service checker
//$search = "(linux|apache|debian|ubuntu|redhat)";
$search = "(microsoft|iis|windows)";
$port = 80;
$timeout = 1;

$stdin = fopen("php://stdin", "r");
function readline($echo="") {
  echo ($echo);
  return trim(fgets($GLOBALS["stdin"]));
}

function get_http_header($host, $port = 80, $timeout = 1) {
  $fp = pfsockopen($host, $port, $errno, $err, $timeout);
  $header = "GET / HTTP/1.1\n\n";
  fwrite($fp, $header);
  $header = "";
  while(!feof($fp) && $fp) {
    $line = fgets($fp);
    if (trim($line) == "") { break; }
    $header = $header.$line;
  }
  fclose($fp);
  return $header;
}

function get_banner($host, $port, $timeout = 1) {
  $fp = pfsockopen($host, $port, $errno, $err, $timeout);
  $header = "CAO-VOE\n\n";
  fwrite($fp, $header);
  stream_set_blocking($fp, 0);
  return(fgets($fp));
}

//die(get_http_header('server00.skola')); //Debug
//die(get_banner('pernicek.mooo.com', $port)); //Debug

error_reporting(0); //Vypneme vypis chyb
while(!feof($stdin) && $stdin) {
while(!feof($stdin) && $stdin) {
  $host = readline(); //Nacteme url
  if($port == 80 || $port == 8080 || $port == 443) {
    $header = get_http_header($host, $port, $timeout);
  } else {
    $header = get_banner($host, $port, $timeout);
  }
  //echo("$header\n"); //Debug
  if ( eregi($search, $header) ) {
    echo("\t!Match: $host:$port\r\n$header\n\n");
  }
  //echo($header); //debug
}