#!/usr/bin/env perl
use strict;
use warnings;
use LWP::Simple;
use threads; # ('exit' => 'threads_only');

sub alrm_sleep {
	my $thr = threads->create(sub {
		$SIG{'TERM'} = sub {
			threads->exit(0);
		};
	}
	$SIG{'ALRM'} = sub {
		$thr->kill('TERM');
	}
	alarm(3);
	$thr->join();
}

my $thr = threads->create(sub {
	$SIG{'KILL'} = sub {
		print("thread is dying\n");
		threads->exit();
	};

	print("new thread\n");
	getprint('http://192.168.2.1:2/');
	print("thread end\n");
});

$thr->kill('SIGKILL');
print("killed thread");
$thr->join();
