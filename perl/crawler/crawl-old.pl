#!/usr/bin/env perl
use strict;
use warnings;
use LWP::Simple qw($ua get);
	$ua->timeout(3);


package WebCrawler;

my $urldb = './urls.txt';

print "This is libwww-perl-$LWP::VERSION\n";

our @urlbuffer;

open(my $fp, "<$urldb");
@urlbuffer = <$fp>;
close($fp);

push(@urlbuffer, 'http://root.cz/');
#push(@urlbuffer, 'http://blog.harvie.cz/');
crawl();

sub uniq(\@) {
  my ($i) = @_;
  my %h;
  @$i = grep(!$h{$_}++, @$i);
  return(@$i);
}


sub status {
	print scalar(@urlbuffer)." buffered URLs\n";
	uniq(@urlbuffer);
	print scalar(@urlbuffer)." buffered URLs\n";

	open(my $fp, ">$urldb");
	print $fp join("\n", @urlbuffer);
	close($fp);
}

sub crawl {
	while(my $_ = shift(@urlbuffer)) {
		status();
		#my $_ = shift(@urlbuffer);
		print "GET $_\n";
		$_ = LWP::Simple::get($_);
		my @urls = /(http:\/\/[_a-zA-Z0-9\.\-]+\.[a-zA-Z]{2,4}\/{1}[-_~&=\ ?\.a-z0-9\/]*)/g; #urls
		#my @urls = /(http:\/\/[^\/'" ]*)/g; #domains
		push(@urlbuffer, uniq(@urls));
		#print "ARRAY: @urlbuffer\n\n";
		#foreach (@urls) { print "$_\n"; }

	}
}

