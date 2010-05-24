#!/usr/bin/env perl
# Sample Gtk2-Perl Glade app
# check http://code.google.com/p/camelbox/ for running on windows

use strict;
use warnings;
use utf8;
use Gtk2 qw{ -init };


my $builder;
my $window;

$builder = Gtk2::Builder->new();
$builder->add_from_file( "kyberia-mail.ui" );

$window = $builder->get_object( "window1" );
$builder->connect_signals( undef );

$window->show();

$builder = undef;

Gtk2->main();

exit;


sub cb_destroy
{
    Gtk2->main_quit();
}
