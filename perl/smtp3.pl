#!/usr/bin/env perl
use strict;
use warnings;
use IO::Socket::INET;

$netdns = eval 'use Net::DNS';
if($netdns) use Net::DNS;

my $from='harvie@harvie.cz';
my $rcpt='harvie@email.cz';
my $subj='Subj3ct';
my $mesg='H3ll0 Dolly!';
$rcpt='martin.mudrunka@t-email.cz'; $mesg = qx { bash -c 'links -dump http://www.gymjosefska.cz/studium/suplovani/suplobec.htm | grep -i "3.AG\\|suplov.n."' };

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

my $data = <<END
HELO $rcpts[1]
MAIL FROM:<$from>
RCPT TO:<$rcpt>
DATA
From: <$from>
To: <$rcpt>
Subject: $subj

$mesg\r\n.\r\nQUIT
END
;

my $sock = IO::Socket::INET->new(PeerAddr => $xchg, PeerPort => '25');
print "SRV: ".<$sock>;
print $data;
print $sock $data;
while(<$sock>) {
	print "SRV: ".$_;
	STDOUT->flush;
}

$sock->close;

