<?php
//HARVESTER IRC BOT
//by: Harvie 2oo7

//Settings:
$server = "irc.2600.net";
$port = "6667";
$channel = "#soom";
$nick = "Harvester";
$jokefile = "jokes.txt";
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
$sfp = fsockopen ($server, $port, &$errno, &$errstr, 5);
irc_connect( $sfp, $nick, $channel, 0 );

$home_channel = $channel;

while(!fwrite($sfp, "")) {

  fwrite($sfp, "JOIN $channel\n");

  //ReadLine from IRC Server
  $line = (fgets($sfp));
  echo($line);
 
  //IRC Sever PING/PONG Response
  $words = (explode(" ", $line));
  if ($words[0] == "PING") {
    fwrite($sfp, "PONG ".trim($words[1])."\n");
    echo("-PONG ".trim($words[1])."\n");
  }
  
  //Prijem pozvanky
  if ( ( trim($words[1]) == "INVITE" ) && ( substr(trim($words[3]), 0, 2) == ":#" ) ) {
      $channel = substr(trim($words[3]), 1);
      fwrite($sfp, "JOIN $channel\n");
      irc_say( $sfp, "Hello", $nick, $channel );
      echo("-JOINED $channel\n");
  }
  
  $commands = (explode(":", $line));
  
  //Harvester - Posle vizitku
  if ( trim($commands[2]) == "Harvester") {
    irc_say( $sfp, ("Ahoj lidi, ja jsem Harvieho bot. Random#: ".rand(0,10)), $nick, $channel );
    echo("-VISITCARD"."\n");
  }
  
  //hspam:Message - Posle zpravu pri kazde prijate
  if ( trim($commands[2]) == "hspam") {
    $spam = trim($commands[3]);
    echo("-SPAM: ".trim($commands[3])."\n");
  }
  
  if ( $spam != "" ) {
    irc_say( $sfp, $spam, $nick, $channel );
    //echo("-SAID: ".trim($commands[3])."\n");
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
    irc_say( $sfp, "Ja jsem Harvester - vice info na: http://ircbot.wz.cz/", $nick, $channel );
    /*
    irc_say( $sfp, "Harvester - Posle vizitku", $nick, $channel );
    irc_say( $sfp, "hhelp - vypise tuto napovedu", $nick, $channel );
    irc_say( $sfp, "hsay:Message - Posle zpravu", $nick, $channel );
    irc_say( $sfp, "hpsay:to:Message - Posle soukromou zpravu kanalu nebo osobe", $nick, $channel );
    irc_say( $sfp, "hdo:Command - Posle serveru prikaz", $nick, $channel );
    irc_say( $sfp, "hmove:Channel - Pripoji do kanalu / Zmeni aktivni kanal", $nick, $channel );
    irc_say( $sfp, "/invite Harvester #channel - Pozve a pripoji bota do kanalu", $nick, $channel );
    irc_say( $sfp, "hpart:Channel - Odpoji se z kanalu", $nick, $channel );
    irc_say( $sfp, "hjoke - Posle \"nahodny\" vtip", $nick, $channel );
    */
    echo("-HELPED"."\n");
  }
  
  //hsay:Message - Posle zpravu
  if ( trim($commands[2]) == "hsay") {
    irc_say( $sfp, trim($commands[3]), $nick, $channel );
    echo("-SAID: ".trim($commands[3])."\n");
  }
  
  //hpsay:to:Message - Posle soukromou zpravu kanalu nebo osobe
  if ( trim($commands[2]) == "hpsay") {
    irc_say( $sfp, trim($commands[4]), $nick, trim($commands[3]) );
    echo("-SAID: ".trim($commands[4])." -- To: ".trim($commands[3])."\n");
  }
  
  //hcol:to:Message - Posle kolizni zpravu kanalu nebo osobe
  if ( trim($commands[2]) == "hcol") {
    irc_say( $sfp, trim($commands[4]), trim($commands[3]), $channel );
    echo("-COLIDED: ".trim($commands[4])." To: ".trim($commands[3])."\n");
  }

  //hdo:Command - Posle serveru prikaz
  if ( trim($commands[2]) == "hdo") {
    $hdo = explode("hdo:", $line);
    fwrite( $sfp, trim($hdo[1])."\n" );
    echo("-DONE: ".trim($hdo[1])."\n");
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

  //hjoke - Posle nahodny vtip
  if  ( trim($commands[2]) == "hjoke") {
      $joke = ($jokes[ rand(0, (sizeof($jokes)-1) ) ]);
      //$ftip = "Potkaji se dve blondyny a jedna rika te druhe: \"How do you do?\" a ta ji odpovi co? kde ses to naucila? To vis vzdycky pretocim radio doprava a tam mluvi jen anglicky tak se to tak ucim. Potkaji se za 2 tydny a jedna rika druhe How do you do a druha ji odpovi chsrchsrsrshchsrshchs";
      irc_say( $sfp, trim($joke), $nick, $channel );
      echo("-JOKED"."\n");
  }

  //Navrat do domaciho kanalu
  $channel = $home_channel;
  
}

echo("-Connection lost.")

?>
