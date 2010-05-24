#!/usr/bin/env perl
use strict;
use warnings;

my @chars=split(//, 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890');
#print $chars[2];

for(my $i=1000;$i<1060;$i++) { for(my $j=0;$j<10170;$j++) {
  my $x0 = $i;
  my $y0 = $j;

  my $x = 0;
  my $y = 0;

  my $iteration = 0;
  my $max_iteration = 1000;
 
  while ( $x*$x + $y*$y <= (2*2)  && $iteration < $max_iteration ) {
    my $xtemp = $x*$x - $y*$y + $x0;
    $y = 2*$x*$y + $y0;
    $x = $xtemp;
    $iteration++;
  }
 
	my $color;
  if ( $iteration == $max_iteration ) {
    $color = 0;
  } else {
    $color = int($iteration/100);
	}
	#print $chars[$color];

  if ( $iteration < $max_iteration/100 ) {
    $color = 0;
  } else {
    $color = 1;
	}
	print $color;

} print "\n"; }
