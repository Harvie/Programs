#!/usr/bin/env perl
use strict;
use warnings;
use threads;
use Thread::Queue;
use threads::shared;
use LWP::Simple qw($ua get);
	$ua->timeout(3);

package WebCrawler;

my $urldb = './urls.txt';

print "This is libwww-perl-$LWP::VERSION\n";

my @urlbuffer :shared;
#open(my $fp, "<$urldb"); @urlbuffer = <$fp>; close($fp); chop(@urlbuffer);
push(@urlbuffer, 'http://root.cz/');
push(@urlbuffer, 'http://blog.harvie.cz/');

sub uniq(\@) {
  my ($i) = @_;
  my %h;
  @$i = grep(!$h{$_}++, @$i);
  return(@$i);
}




my $threads = 10;
for(my $i=0; $i<$threads; $i++) {
	my $thr = threads->create(sub {
		print("[*] Worker #$i running!\n");

		while (1) {
			lock(@urlbuffer); shift(@urlbuffer);
			if(length($_) < 3) { sleep(1); next; }

			print "#$i:\tGET $_\n";
			$_ = LWP::Simple::get($_);

			my @urls;
			if($_) {
				@urls = /(http:\/\/[_a-zA-Z0-9\.\-]+\.[a-zA-Z]{2,4}\/{1}[-_~&=\ ?\.a-z0-9\/]*)/g; #urls
				#@urls = /(http:\/\/[^\/'" ]*)/g; #domains
				if($i && @urlbuffer < 1000) {
					#print("#$i:\tENQ: @urls\n");
					lock(@urlbuffer);
					push(@urlbuffer, uniq(@urls));
					#while(uniq(@urls)) { push(@urlbuffer, $_); }
				} 
			}
		}

		print("[*] Worker #$i stopped!\n");
	}) -> detach();
}

while(1) {
	print '###: '.@urlbuffer."\n";
	lock(@urlbuffer); uniq(@urlbuffer);
	print '###: '.@urlbuffer."\n";

	open(my $fp, ">$urldb");
	print $fp join("\n", @urlbuffer);
	close($fp);

	sleep(5);
}
