#!/usr/bin/env perl
#pacman -S perl-curses-ui

use utf8;
use I18N::Collate;
setlocale('LC_COLLATE', 'cs_CZ.utf8');
use encoding 'UTF-8';

use warnings;
use strict;

use Curses::UI;

our $path = "/home";
our $file = "";

my $cui = new Curses::UI(-color_support => 1, -language => "czech", -utf8 => 1);
my @menu = (
    {
         -label => 'Soubor', -submenu =>
            [
                {-label => 'Nacist  Ctrl-O', -value => \&open_file},
                {-label => 'Ulozit  Ctrl-S', -value => \&save},
                {-label => 'Konec   Ctrl-X', -value => \&exit_dialog}
            ]
    },
    {
         -label => 'Napoveda', -submenu =>
            [
                {-label => 'O aplikaci', -value => \&about},
            ]
    },
);

#menu
my $menu = $cui->add(
    "menu",
    "Menubar",
    -menu =>\@menu,
    -fg => "white",
    -bg => "blue"
);


my $win = $cui->add(
    "win",
    "Window",
    -y => 2,
    -x => 0,
    -border => 0
);

my $label = $win->add(
    "label", "Label",
    -x     => 0,
    -y     => 0,
    -bold  => 1,
);

my $texteditor = $win->add(
    "text",
    "TextEditor",
    -hscrollbar => 1,
    -vscrollbar => 1,-y=>2,
    -border=>1
);
$texteditor->focus();

open_file($ARGV[0]) if $ARGV[0] && (-r $ARGV[0]);


$cui->set_binding(\&open_file, "\cO");
$cui->set_binding(\&save, "\cS");
$cui->set_binding(\&exit_dialog, "\cX");
$cui->set_binding(sub {$menu->focus();}, "\t");

$cui->mainloop();

sub exit_dialog {
    my $return = $cui->dialog(
        -message   => "Chcete ukoncit program?",
        -title     => "Jste si jisty?",
        -buttons   => ['yes', 'no'],
    );
    exit(0) if $return;
}

sub open_file {
    my $soubor = shift;
    $soubor = $cui->loadfilebrowser(
        -path => $path
    ) unless -r $soubor;

    if($soubor and -r $soubor and -T $soubor){
        local $/ = undef;
        open DATA, $soubor;
        my $data = <DATA>;
        $texteditor->text($data);
        close DATA;
        ($path) = $soubor =~ /^(.*\/).*$/;
        $file = $soubor;
        $label->text("Soubor: ".$file);
    }else{
        $cui->error("Nevybral jste soubor");
    }
    return $soubor;
}

sub save {
    my $soubor = $cui->savefilebrowser(
        -path => $path,
    );

    if(defined $soubor){
        local $/ = undef;
        open CIL, ">$soubor";
        print CIL $texteditor->get();
        close CIL;
        ($path) = $soubor =~ /^(.*\/).*$/;
        $file = $soubor;
        $label->text("Soubor: ".$file);
    }else{
        $cui->error("Nevybral jste soubor");
    }
}

sub about {
    $cui->dialog("Toto je napoveda");
}

