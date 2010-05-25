#!/usr/bin/php
<?php

srand(time()+rand(0,9));

while(1) echo rand(0,20)."\n";

function rnd() {
	while( ($i = (rand(0,1000000000)/1000000000)) == 1 );
	return $i;
}

while( ($i = round(rnd()*15)) != 15) {
	echo $i."\n";
}
echo $i."\n";