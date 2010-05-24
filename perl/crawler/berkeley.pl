#!/usr/bin/env perl
use strict;
use warnings;
use threads; # ('exit' => 'threads_only');
use BerkeleyDB; #man 3pm BerkeleyDB



our $dbenv = BerkeleyDB::Env->new(
	-Home => './',
	-Flags =>  DB_CREATE|DB_INIT_CDB|DB_INIT_MPOOL
) or die("cannot open environment: $BerkeleyDB::Error\n");

our $dbq   = BerkeleyDB::Queue->new(
	-Filename => 'urls.queue' ,
	-Flags      => DB_CREATE,
	-Len        => 32,
	-Env        => $dbenv
) or die("cannot open queue: $BerkeleyDB::Error\n");

for(my $i=0;$i<10;$i++) { $dbq->db_put($i, 'toto'); }


my $threads = 2;
for(my $i=0; $i<$threads; $i++) {
	my $thr = threads->create(sub {
		print("[*] Worker #$i running!\n");
		my $key=0;
		my $val='';
		my $cursor = $dbq->db_cursor() ;
		while ($cursor->c_get($key, $val, DB_NEXT) == 0) {
			#print "$key -> $val\n"
			sleep(1);
			if($val) { print("[$i] $key = $val\n"); }
		}
		#$dbq->db_get($key, $val, DB_CONSUME);
		print("[*] Worker #$i stopped!\n");
	}) -> detach();
}

sleep(3);


