#!/usr/bin/perl

use strict;
use warnings;
use Math::Trig;

#$radians = deg2rad($degrees);
#$degrees = rad2deg($radians);

print "3.14159265358979323846264338327950288419716939937510\n";
#for (my $c=1;$c>0;$c++) {
for (my $c=271842800;$c>0;$c++) {
	printf '%.80f', ($c * sin ( deg2rad( 180 / $c ) ));
	print "                   " . $c . "            \r";
}

#my $pi = (4 * atan2(1,1));
#print $pi;
