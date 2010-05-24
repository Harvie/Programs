#!/usr/bin/env perl
use strict;
use warnings;
use IO::Socket::INET;
use Net::SMTP;

my $netdns = eval 'use Net::DNS';
if($netdns) { use Net::DNS };

my $from='harvie@harvie.cz';
my $rcpt='harvie@email.cz';
my $subj='Subj3ct';
my $mesg='H3ll0 Dolly!';

my @rcpts = split('@', $rcpt);


if($netdns) {
	my $preference = -1;
	my $xchg;
	foreach(mx($rcpts[1])) {
		if($_->preference > $preference) {
			$preference = $_->preference;
			$xchg = $_->exchange;
		}
	}
} else {
	#get mx without Net::DNS
	foreach(split("\n", qx{ bash -c "host -t mx email.cz | cut -d ' ' -f 7" })) { 
		chop($_);
		print "mx: $_\n";
	}
}


my $smtp = Net::SMTP->new('mx50.seznam.cz', ( debug => 1));

$smtp->mail($from);
$smtp->to($rcpt);

$smtp->data();
$smtp->datasend("Subject: $subj\n");
$smtp->datasend("From: $from\n");
$smtp->datasend("To: $rcpt\n");
$smtp->datasend("\n");
$smtp->datasend("$mesg\n");
$smtp->dataend();
$smtp->quit;

