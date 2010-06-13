#!/usr/bin/env perl
use strict;
use warnings;

use JSON;
use Data::Dumper;

my $a={a=>{f=>"",b=>""},b=>{f=>"",b=>""}};
$a->{"b"}=$a;

print Dumper($a);
print encode_json($a)."\n"
