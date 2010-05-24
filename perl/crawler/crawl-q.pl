#!/usr/bin/env perl
use strict;
use warnings;
use threads;
use Thread::Queue;
use LWP::Simple qw($ua get);
	$ua->timeout(3);


package WebCrawler;

my $urldb = './urls.txt';

print "This is libwww-perl-$LWP::VERSION\n";

my $urlqueue = Thread::Queue->new();
#open(my $fp, "<$urldb"); while(<$fp>) { $urlqueue->enqueue($_); } close($fp);
$urlqueue->enqueue("http://root.cz/");
$urlqueue->enqueue("http://blog.harvie.cz/");

sub uniq(\@) {
  my ($i) = @_;
  my %h;
  @$i = grep(!$h{$_}++, @$i);
  return(@$i);
}

use Data::Dumper;
#die(Dumper($urlqueue));
while(my @urlqueue) {
	print($_."\n");
} die;

my $threads = 20;
for(my $i=0; $i<$threads; $i++) {
	my $thr = threads->create(sub {
		print("[*] Worker #$i running!\n");

		while (my $url = $urlqueue->dequeue()) {
			#$urlqueue->enqueue($url);
			print "#$i:\tGET $url\n";
			$_ = LWP::Simple::get($url);
			my @urls;
			if($_) {
				@urls = /(http:\/\/[_a-zA-Z0-9\.\-]+\.[a-zA-Z]{2,4}\/{1}[-_~&=\ ?\.a-z0-9\/]*)/g; #urls
				#@urls = /(http:\/\/[^\/'" ]*)/g; #domains
				if($urlqueue->pending() < 1000) {
					#print("#$i:\tENQ: @urls\n");
					$urlqueue->enqueue(uniq(@urls));
					#while(uniq(@urls)) { $urlqueue->enqueue($_); }
				}
			}
		}

		print("[*] Worker #$i stopped!\n");
	}) -> detach();
}

while(1) {
	print '###: '.$urlqueue->pending()."\n";
	sleep(3);
}
