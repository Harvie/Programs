#!/usr/bin/env perl
use strict;
use warnings;

my $period = int(27.321661*24*60*60); #Period of moon rotation in seconds
$period = 2571720;
my $newmoon = 366382697; #Some newmoon in history
my $fullmoon = 365613152; #Some fullmoon in history (14-15d after newmoon)
$fullmoon = 1106645520;
my $now = time();

while($fullmoon < $now) {
	$fullmoon += $period;
}
$fullmoon = int($fullmoon);

my $fullmoon_remaining = ($fullmoon-$now)/60/60/24;

print "$fullmoon = ".localtime($fullmoon)." = ".$fullmoon_remaining." days remaining\n";
#1254629460

