#!/usr/bin/env perl
use Digest::MD5 md5_hex;

print "Ahoj svetee!\n";

@array = ("Neo", "Trinity", "Morpheus");
foreach(@array) { print( md5_hex($_).": $_\n"); }

for($i=1;$i<100000;$i++) { print(md5_hex($i*int(rand(10)))."\r") }

print "\n";
