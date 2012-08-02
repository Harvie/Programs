#!/usr/bin/env perl
#This script processes output of goertzel and paints histogram

use strict;
use warnings;
use Term::ReadKey;


sub bar($$$) {
	my ($value,$max,$prefix) = @_;
	my ($wchar, $hchar, $wpixels, $hpixels) = GetTerminalSize();
	$value = ($value/$max)*($wchar-length($prefix));
	return $prefix."#" x $value." " x ($wchar-$value-length($prefix));
}

sub trim($) {
	my $string = shift;
	$string =~ s/^\s+//;
	$string =~ s/\s+$//;
	return $string;
}

print "\033[H\033[2J";

my @header = split('\t', <STDIN>);

while(<STDIN>) {
	my @values = split('\t', $_);
	my $i=0;
	print "\033[H";
	foreach my $i (1..$#values) {
		print bar($values[$i],10,sprintf("%8s ",trim($header[$i])))."\n";
	}
	print "Time: ".$values[0]."                   \n";
	print "                           \n";
}

exit 0;
