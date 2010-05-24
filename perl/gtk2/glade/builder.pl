#!/usr/bin/env perl
# Sample Gtk2-Perl Glade app
# check http://code.google.com/p/camelbox/ for running on windows

use strict;
use warnings;
use utf8;
use Gtk2 qw{ -init };

my $builder = Gtk2::Builder->new();
$builder->add_from_file( "sample.builder" );

my %widgets;
foreach my $widget qw(window1 label1 entry1)
{
	$widgets{$widget}=$builder->get_object($widget);
}

$builder->connect_signals( undef );
$builder = undef;

$widgets{'window1'}->show();

Gtk2->main();
exit;


sub on_button1_clicked
{
	$widgets{'label1'}->set_text($widgets{'entry1'}->get_text());
	print 'Label1 text set to: '.$widgets{'label1'}->get_text()."\n";
}


sub cb_destroy
{
    Gtk2->main_quit();
}
