#!/usr/bin/env perl
use strict;
use warnings;

my $sub = q {{
	print "rofl\n";
}};

print "$sub\n";
eval $sub;
eval "sub foo $sub"; foo();
