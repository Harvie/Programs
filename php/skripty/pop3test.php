<?php

$server = "pop3.seznam.cz";
$port = "110";
$timeout = "5";
$user = "harvie@seznam.cz";
$pass = "heslo";

$fp = fsockopen ($server, $port, &$errno, &$errstr, $timeout);
if ($fp) { echo("GOOD CONNECTION\n"); }
if (!$fp) { echo("BAD CONNECTION\n"); }

$resp = (" ".fread($fp, 1000));
echo($resp);
if (strpos($resp, "+OK") != 0) { echo("GOOD SERVER\n"); }
if (strpos($resp, "-ERR") != 0) { echo("BAD SERVER\n"); }

fwrite($fp, "USER $user\n");
$resp = (" ".fread($fp, 1000));
echo($resp);
if (strpos($resp, "+OK") != 0) { echo("GOOD USERNAME\n"); }
if (strpos($resp, "-ERR") != 0) { echo("BAD USERNAME\n"); }

fwrite($fp, "PASS $pass\n");
$resp = (" ".fread($fp, 1000));
echo($resp);
if (strpos($resp, "+OK") != 0) { echo("GOOD PASSWORD\n"); }
if (strpos($resp, "-ERR") != 0) { echo("BAD PASSWORD\n"); }

if(fclose($fp)) { echo("CONNECTION CLOSED\n"); } else
                { echo("CAN'T CLOSE CONNECTION\n"); }

?>
