#!/usr/bin/env perl
use strict;
use warnings;
use Gtk2 -init;

# Gtk2->init; works if you didn't use -init on use
my $window = Gtk2::Window->new ('toplevel');
my $button = Gtk2::Button->new ('Quit');
$button->signal_connect (clicked => sub { Gtk2->main_quit });
$window->add ($button);
$window->show_all;
Gtk2->main;

