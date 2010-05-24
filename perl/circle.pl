#!/usr/bin/perl
use strict;
use warnings;
use Math::Trig;

my $r = 20; #radius
my $zoom = 2; #x zoom

for (my $y=-$r;$y<=$r;$y++) {
	my @line = split(//," "x(2*$zoom*$r));
	my $x = int(sqrt(($r*$r)-($y*$y))+.5);
	$line[($r-$x)*$zoom] = "+";
	$line[($r+$x)*$zoom] = "+";
	print @line; print "\n";
}
