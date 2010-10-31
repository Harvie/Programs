#!/usr/bin/env perl
#ArchLinux Daemon Manager Gui! by Harvie 2oo9
use strict;
use warnings;
use utf8;
require "MPlayer.pm";
use Gtk2 qw(-init);

my $player = Audio::Play::MPlayer->new;
$player->load( "/home/harvie/Downloads/Shared/Bonobo-Walk_In_The_Sky_(Feat.Bajka).mp3" );
sleep 1;
#print $player->title. "\n";
#print $player->{track}->{title}, "\n";
$player->speed(0.3);
$player->poll( 1 ) until $player->state == 0;

my $builder = Gtk2::Builder->new();
$builder->add_from_file('mpmix.ui');

my %widgets;
foreach my $widget qw(window1)
{
	$widgets{$widget}=$builder->get_object($widget);
}

$builder->connect_signals(undef);
$builder = undef;

$widgets{'window1'}->show();


Gtk2->main();
exit;

sub on_window1_destroy
{
    Gtk2->main_quit();
}
