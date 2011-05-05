#!/usr/bin/env perl
use strict;
use warnings;
use threads;
#use Thread::Queue;
#use threads::shared;
use LWP::Simple qw($ua get); $ua->timeout(3);
use BerkeleyDB;

print STDERR "This is libwww-perl-$LWP::VERSION\n";

#my @queue :shared;
#my %crawled :shared;
tie my @queue, "BerkeleyDB::Recno", -Filename => 'queue.db', -Flags => DB_CREATE || die "Cannot open DB1!\n";
tie my %crawled, "BerkeleyDB::Hash", -Filename => 'urls.db', -Flags => DB_CREATE || die "Cannot open DB2!\n";

push(@queue, 'http://root.cz/');
push(@queue, 'http://blog.harvie.cz/');

untie @queue;
untie %crawled;


sub uniq(\@) {
  my ($i) = @_;
  my %h;
  @$i = grep(!$h{$_}++, @$i);
  return(@$i);
}

my $threads = 3;
for(my $i=0; $i<$threads; $i++) {
	my $thr = threads->create(sub {
		print("[*] Worker #$i running!\n");

           my $env = new BerkeleyDB::Env
                         -Home   => "/tmp/" ,
                         -Flags  => DB_CREATE| DB_INIT_CDB | DB_INIT_MPOOL
               or die "cannot open environment: $BerkeleyDB::Error\n";


		tie my @queue, "BerkeleyDB::Recno", -Filename => 'queue.db', -Flags => DB_CREATE, -Env => $env || die "Cannot open DB1!\n";
		tie my %crawled, "BerkeleyDB::Hash", -Filename => 'urls.db', -Flags => DB_CREATE, -Env => $env || die "Cannot open DB2!\n";

		while (1) {
			print "omg ".shift(@queue)."\n";
			if(length($_) < 3) { sleep(1); next; }

			print "#$i:\tGET $_\n";
			$_ = LWP::Simple::get($_);

			my @urls;
			if($_) {
				@urls = /(http:\/\/[_a-zA-Z0-9\.\-]+\.[a-zA-Z]{2,4}\/{1}[-_~&=\ ?\.a-z0-9\/]*)/g; #urls
				#@urls = /(http:\/\/[^\/'" ]*)/g; #domains
				if($i && @queue < 1000) {
					#print("#$i:\tENQ: @urls\n");
					push(@queue, uniq(@urls));
					#while(uniq(@urls)) { push(@queue, $_); }
				} 
			}
		}

		print("[*] Worker #$i stopped!\n");
	}) -> detach();
}

while(1) {
	print '###: '.@queue."\n";
	uniq(@queue);
	print '###: '.@queue."\n";

	sleep(5);
}
