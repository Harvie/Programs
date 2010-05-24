#!/usr/bin/env perl
use strict;
use warnings;
use File::Find;
use URI::Escape;

my $index_dir = './index';
my $docus_dir = './docs';

unlink <$index_dir/*>;
find({ wanted => \&index_file, no_chdir=>1 }, $docus_dir);

sub index_file() {
	print $File::Find::name."\n";
	open(my $fh, '<', $File::Find::name) or die $!;
	while(<$fh>) {
		$_ =~ s/\<[^\<]+\>//g; #strip tags
		$_ = lc($_);

		foreach(split(/[\s]/,$_)) {
			$_ =~ /^\s+|\s+$/g; #trim
				if($_ eq '') { next; }
	
			print "$_\n";
			$_ = uri_escape($_);
			$_ = $index_dir.'/'.$_;
	
				open(my $ifh, '>>', $_) or die $!;
			print $ifh "$File::Find::name\n";
			close($ifh);
		}
	}
	close($fh);
}
