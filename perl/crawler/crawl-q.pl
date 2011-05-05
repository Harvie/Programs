#!/usr/bin/env perl
package WebCrawler;

use strict;
use warnings;
use threads;
use Thread::Queue;
use BerkeleyDB;
use LWP::Simple qw($ua get); $ua->timeout(3);

print "This is libwww-perl-$LWP::VERSION\n";


my $urlqueue = Thread::Queue->new();
$urlqueue->enqueue("http://root.cz/");
$urlqueue->enqueue("http://blog.harvie.cz/");

sub uniq(\@) {
  my ($i) = @_;
  my %h;
  @$i = grep(!$h{$_}++, @$i);
  return(@$i);
}

sub crawl_url {
#sub crawl_url($ \%) {
	my ($url, $crawled) = @_;
	#use Data::Dumper; print(Dumper(%$crawled)."\n");
	print "#\tGET $url\n";
	$_ = LWP::Simple::get($url) || return;
	%$crawled->{$url}=1;
	my @urls;
	if($_) {
		@urls = /(http:\/\/[_a-zA-Z0-9\.\-]+\.[a-zA-Z]{2,4}\/{1}[-_~&=\ ?\.a-z0-9\/]*)/g; #urls
		#@urls = /(http:\/\/[^\/'" ]*)/g; #domains
		return uniq(@urls);
	}
}

#use Data::Dumper;
#die(Dumper($urlqueue));
#while(my @urlqueue) {
#	print($_."\n");
#} die;

my $threads = 3;
for(my $i=0; $i<$threads; $i++) {
	my $thr = threads->create(sub {
		print("[*] Worker #$i running!\n");

		my $env = new BerkeleyDB::Env -Home => "/tmp/", -Flags  => DB_CREATE| DB_INIT_CDB | DB_INIT_MPOOL || die "cannot open environment: $BerkeleyDB::Error\n";
		my $db = tie my %crawled, "BerkeleyDB::Hash", -Filename => 'urls.db', -Flags => DB_CREATE, -Env => $env || die "Cannot open DB!\n";

		while (my $url = $urlqueue->dequeue()) {
			#print "#$i:\tGET $url\n";
			my @urls = crawl_url($url, \%crawled);
			$db->sync();
			if($urlqueue->pending() < 1000) {
				#print("#$i:\tENQ: @urls\n");
				$urlqueue->enqueue(@urls);
			}
		}

		print("[*] Worker #$i stopped!\n");
	}) -> detach();
}

while(1) {
	print '###: '.$urlqueue->pending()."\n";
	sleep(3);
}
