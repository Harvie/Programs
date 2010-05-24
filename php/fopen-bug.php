#!/usr/bin/php
<?php

//phpinfo(); die();

/*Run and wait for a while, this can totaly stop the script at the dead
point...*/

$i=1;
ini_set('default_socket_timeout',1);
set_time_limit(0);
$url='http://ad.doubleclick.net/click';
$url='http://w.moreover.com/';
//$url = 'http://pernicek.mooo.com';
while(1) {
    @file_get_contents($url, false, null, 0, 10000);
    echo "#".$i++;
}

?>

