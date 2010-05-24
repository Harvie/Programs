#!/usr/bin/env perl
use strict;
use warnings;

package WebCrawler;

sub uniq(\@) { 
	my ($i) = @_;
  my %h;
  @h{@$i} = ();
  @$i = keys %h;
	return(@$i);
}

sub unique(\@) { 
	my ($i) = @_;
	my %h;
	@$i = grep(!$h{$_}++, @$i);
	return(@$i);
}

my @b = qw(b b c c c a a d);
print "@b\n";
unique(@b);
print "@b\n";
