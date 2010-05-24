#!/usr/bin/perl

$half = 1600;

$line = `xwarppointer get`;
chomp $line;
@c = split(/ /, $line);

if ($c[0] > $half) {
    $c[0] -= $half;
} else {
    $c[0] += $half;
}

`xwarppointer abspos $c[0] .`;
