#!/usr/bin/env perl
use strict;
use warnings;
use LWP::Simple;
use XML::Simple;
use Data::Dumper;

binmode(STDOUT, ":encoding(utf8)");

#print get('http://localhost/');

my $xml = new XML::Simple(ForceArray => 1);
my $data = $xml->XMLin(get('http://blog.harvie.cz/rss/'));
#print Dumper($data); #debug
foreach( @{$data->{channel}[0]->{item}} ) {	print("$_->{title}[0]\n\t$_->{link}[0]\n");	}
