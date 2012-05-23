#!/usr/bin/env perl
use Fuse::Simple qw(accessor main nocache saferun runcode);
#use threads;
#use threads::shared;

my $randfile = sub {
	nocache $_[0]." ".rand(23)."\n";
};


my $randdir = sub {
		my $list = {};
		for(my $i=0;$i<int(rand(15));$i++) {
			#$list->{$i}="test #$i\n";
			$list->{$i}=saferun($randfile, $i);
		}
		nocache $list;
};

my $var = "this is a variable you can modify. write to me!\n";
my $filesystem = {	
	list => sub {
		nocache("LOOOL:".rand(9999)."\n");
	},
	listing => $randdir,
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

print "mount....\n";
main(
  #"mountpoint" => "./test",      # actually optional
  "debug"      => 0,           # for debugging Fuse::Simple. optional
  "fuse_debug" => 0,           # for debugging FUSE itself. optional
  "threaded"   => 0,           # optional
  "/"          => $filesystem, # required :-)
);
print "unmounted\n";
