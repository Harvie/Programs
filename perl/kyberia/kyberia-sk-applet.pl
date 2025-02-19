#!/usr/bin/env perl
#Kyberia.sk-Mail-Applet (Harvie 2oo9)
use strict;
use Gtk2 -init;
require "Kyberia.pl";

my $user = 'Harvie';
my $pass = 'Beruska';
my $url = 'https://kyberia.sk/';

my $kyberia = Kyberia->new($url);
$kyberia->Login($user, $pass);
my $icon_new = 'gnome-stock-mail-new'; #New mail
my $icon_old = 'mail-reply-all'; #No mail
my $statusicon = Gtk2::StatusIcon->new();

$statusicon->signal_connect(activate => sub {
	system("gnome-open $url/id/24 &");
	alarm(4);
});

my $state = -1;
$SIG{'ALRM'} = sub {
	my $last = $state;
	my($state, $name, $text) = $kyberia->CheckMail();
	if($state != $last) {
		$statusicon->set_blinking($state);
		$statusicon->set_tooltip_text($text);
		if($state >= 1) {
			$statusicon->set_from_icon_name($icon_new);
		} else {
			$statusicon->set_from_icon_name($icon_old);
		}
	}
	alarm(30);
};
alarm(1);

Gtk2->main;
