#!/usr/bin/env perl
use strict;
use warnings;
use Gtk2 -init;

my $window = Gtk2::Window->new ('toplevel');
$window->set_default_size(640, 480);
$window->signal_connect( destroy => sub {
    Gtk2->main_quit();
});

my $xterm = new Gtk2::Socket;
$xterm->signal_connect_after( realize => sub {
   my $xid = $_[0]->window->get_xid;
   system "xterm -bg black -fg white -into $xid -e 'ls -l; bash' &";
});

$window->add($xterm);
$window->show_all;
Gtk2->main;
