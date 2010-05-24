#!/usr/bin/env perl
use Fuse::Simple qw(accessor main);
#use threads;
#use threads::shared;

my $var = "this is a variable you can modify. write to me!\n";
my $filesystem = {
  foo => "this is the contents of a file called foo\n",
  subdir => {
    "foo"  => "this foo is in a subdir called subdir\n",
    "blah" => "this blah is in a subdir called subdir\n",
  },
  "blah" => \ "subdir/blah",        # scalar refs are symlinks
	recursion => {
  	"recursion" => \ ".",        # scalar refs are symlinks
	},
  "magic" => sub { return "42\n" }, # will be called to get value
  "var"  => accessor(\$var),        # read and write this variable
  "var2" => accessor(\$var),        # and the same variable
  "var.b" => accessor(\ my $tmp),   # and an anonymous var
};
main(
  #"mountpoint" => "./test",      # actually optional
  "debug"      => 0,           # for debugging Fuse::Simple. optional
  "fuse_debug" => 0,           # for debugging FUSE itself. optional
  "threaded"   => 0,           # optional
  "/"          => $filesystem, # required :-)
);
