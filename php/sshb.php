#!/usr/bin/php
<?php

$user = 'harvie';
$host = 'harvie.ath.cx';
$port = 22;

$stdin = fopen('php://stdin', 'r');
while(!feof($stdin)) {
	$pass = trim(fgets($stdin));
	$ssh = ssh2_connect($host, $port);
	if (ssh2_auth_password($connection, $user, $pass)) {
		echo("Authentication Successful: $pass\n");
	} else {
		die("Authentication Failed...\n");
	}
}