#!/usr/bin/env perl
use strict;
use warnings;
use LWP::Simple;
use Time::HiRes qw(alarm ualarm);

while(1) {
	$SIG{ALRM} = sub {
		alarm(1);
		print("again\n");
		getprint('http://192.168.2.1:2/');
		#print get('http://192.168.2.1:2/');
	};
	alarm(1);
	sleep(1);
}
