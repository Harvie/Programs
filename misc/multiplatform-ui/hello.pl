#!/usr/bin/env perl
#GTK Hello world! by Harvie 2oo9

use strict;
use warnings;
use utf8;
use Gtk2 qw(-init);

my $builder = Gtk2::Builder->new();
$builder->add_from_file('hello.ui');

my %widgets;
foreach my $widget qw(window1 button1 entry1)
{
	$widgets{$widget}=$builder->get_object($widget);
}

$builder->connect_signals( undef );
$builder = undef;

$widgets{'window1'}->show();

Gtk2->main();
exit;

sub on_window1_destroy
{
    Gtk2->main_quit();
}

sub on_button1_clicked
{
	$widgets{'button1'}->set_label($widgets{'entry1'}->get_text());
	print $widgets{'entry1'}->get_text()."\n";
}



