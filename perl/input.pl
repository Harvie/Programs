#!/usr/bin/env perl
use strict;
use warnings;
use Digest::MD5;
my $in = <>;
chop($in);
print Digest::MD5->md5_hex($in)."\n";

