#!/usr/bin/env perl
use strict;
use warnings;
use IO::Socket::INET;
use LWP::Simple;

our $g_client = 'petella';
our $g_client_version = '0.1';
$g_client = 'LimeWire';
$g_client_version = '5.1.2';
our %g_cache;

#my $sock = IO::Socket::INET->new(PeerAddr => 'localhost', PeerPort => '80');
#my $sock = IO::Socket::INET->new('localhost:80');
#print $sock "GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
#while(<$sock>) { print $_; STDOUT->flush; } $sock->close;

my $gwc_url = 'http://gwc.lame.net/gwcii.php';
foreach( split(/\n/, get($gwc_url."?client=$g_client&version=$g_client_version&hostfile=1")) ) {
	my($ip, $port) = split(/:/, $_);
	$ip =~ s/[^0-9\.]//;
	$port =~ s/[^0-9]//;
	$g_cache{$ip}=$port;
}

while (my($ip, $port) = each(%g_cache)){
	print "connecting to gnutella:$ip:$port\n";
	#my $sock = IO::Socket::INET->new('10.127.85.96:35301');
	my $sock = IO::Socket::INET->new(PeerAddr => $ip, PeerPort => $port);
	print $sock "GNUTELLA CONNECT/0.6\r\n".
		"User-Agent: $g_client/$g_client_version\r\n".
		"X-Ultrapeer: False\r\n".
		"X-Query-Routing: 0.1\r\n\r\n";
	while(<$sock>) {
		print $_;
		STDOUT->flush;
	}
	$sock->close;
}



