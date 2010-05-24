#!/usr/bin/env perl

my ($dataset, $outdir, $prepared);
$dataset='set_small.dat.bz2';
$dataset='set_large.dat.bz2';

$outdir='/tmp/eset';
$prepared="$outdir/test";

print "preparing...\n";
system("mkdir -p $outdir");
system("bzip2 -dc '$dataset' | cut -d ' ' -f 2- | sort -u > '$prepared'");
print "running...\n";

