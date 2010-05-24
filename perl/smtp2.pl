#!/usr/bin/env perl
use strict;
use warnings;
#use IO::Socket::INET;
#use Net::DNS;

use Net::SMTP;

           my $smtp = Net::SMTP->new('mx50.seznam.cz');

           $smtp->mail('harvie@harvie.cz');
           $smtp->to('harvie@email.cz');

           $smtp->data();
           $smtp->datasend("Subject: testttt\n");
           $smtp->datasend("From: tomas\@mudrunka.cz\n");
           $smtp->datasend("To: harvie\@email.cz\n");
           $smtp->datasend("\n");
           $smtp->datasend("A simple test message\n");
           $smtp->dataend();

           $smtp->quit;

