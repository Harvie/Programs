<?php
$serv = "pop3.seznam.cz";
$port = "110";
$mail = "harvie@seznam.cz";
$pass = "harddaysnight";
$delay = 5;

///FUNCS///////////////////////////////////////////////////////////////

function pop3_connect($mail, $pass, $serv, $port=110) {
  $fserv = fsockopen($serv, $port);
  if(!$fserv) return -1; //Cannot connect
  $resp = fgets($fserv); //echo($resp); //Debug
  $request = "USER $mail\nPASS $pass\n";
  
  fwrite($fserv, $request);
  $resp = fgets($fserv); if (!eregi("\\+", $resp)) return -2; //Bad username
  //echo($resp);
  $resp = fgets($fserv); if (!eregi("\\+", $resp)) return -3; //Bad password 
  //echo($resp);

  return($fserv);
}

function pop3_cmd($fserv, $cmd) {
  $request = "$cmd\n";
  fwrite($fserv, $request);
  return trim(fgets($fserv));
}

function pop3_check($fserv) {
  echo($fserv."\n");
  switch($fserv) {
    case -1: echo("Cannot conect!\n"); break;
    case -2: echo("Bad username!\n"); break;
    case -3: echo("Bad password!\n"); break;
  }
}

function pop3_unread($fserv) {
  $resp = pop3_cmd($fserv, "STAT");
  $resp = explode(" ", $resp);
  return trim($resp[1]);
}

function pop3_quit($fserv) {
  pop3_cmd($fserv, "QUIT");
  fclose($fserv);
}
///CODE/////////////////////////////////////////////


//pop3_check($fserv);
//echo pop3_cmd($fserv, "STAT")."\n";

$last_unread = 0;
while(1) {
  if(!isset($fserv) || !$fserv) {
    $fserv = pop3_connect($mail, $pass, $serv, $port);
  }

  $unread = pop3_unread($fserv);
  if($unread != $last_unread && $unread != "") {
    echo("You have $unread messages!\n");
    $last_unread = $unread;
  }

  pop3_quit($fserv);
  sleep($delay);
}

/*
USER harvie@seznam.cz
PASS harddaysnight
*/

/*
+OK Hello, this is Seznam POP3 server 2.3.16.
USER harvie@seznam.cz
PASS harddaysnight
+OK Enter your password please.
+OK 79 1298041
*/
