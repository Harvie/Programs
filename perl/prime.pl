#!/usr/bin/perl
# prime-pthread, courtesy of Tom Christiansen

use strict;
use warnings;

use threads;
use Thread::Queue;

my $stream = Thread::Queue->new();
for my $i ( 3 .. 1000 ) {
    $stream->enqueue($i);
}
$stream->enqueue(undef);

threads->create(\&check_num, $stream, 2);
$kid->join();

sub check_num {
    my ($upstream, $cur_prime) = @_;
    my $kid;
    my $downstream = Thread::Queue->new();
    while (my $num = $upstream->dequeue()) {
        next unless ($num % $cur_prime);
        if ($kid) {
            $downstream->enqueue($num);
        } else {
            print("Found prime $num\n");
            $kid = threads->create(\&check_num, $downstream, $num);
        }
    }
    if ($kid) {
        $downstream->enqueue(undef);
        $kid->join();
    }
}
