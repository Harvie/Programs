/*
Generates Ascii table (By Harvie 2oo6)
*/

<?php

echo ("Ascii table (By Harvie 2oo6)\n\n");


$chr_in = 0;

while ( $chr_in < 256 ) {

echo ($chr_in . " - " . chr ( $chr_in ) . "\n");
$chr_in++;

}

?>
