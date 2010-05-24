#!/usr/bin/env perl
use strict;
use warnings;
use IO::Socket::INET;

my $sock = IO::Socket::INET->new('localhost:80');
print $sock "GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
while(<$sock>) {
	print $_;
	STDOUT->flush;
}

$sock->close;

