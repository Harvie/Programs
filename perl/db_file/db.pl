#!/usr/bin/env perl
use strict;
use warnings;
use BerkeleyDB;

#my ( %h $k $v );
tie my %h, "BerkeleyDB::Hash", -Filename => '_fruit.db', -Flags => DB_CREATE || die "Cannot open DB!\n";
#tie my %h, "Tie::Hash::DBD", "dbi:SQLite:dbname=_sqlite.db";

# Add a few key/value pairs to the file
$h{"apple"} = "red";
#$h{"banana"} = "yellow";
$h{"orange"} = "orange";
$h{"tomato"} = "red";

use JSON;
use Data::Dumper;
#$h{"test"} = to_json({"from" => "to", "lol" => "rofl"});
print $h{"test"}."\n";
print Dumper(from_json($h{"test"}))."\n";


# Check for existence of a key
print "Banana Exists\n\n" if $h{"banana"};

# Delete a key/value pair.
delete $h{"apple"};

# print the contents of the file
while (my ($k, $v) = each %h) { print "$k -> $v\n" }

untie %h;
