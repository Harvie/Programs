#!/usr/bin/env perl

           use Curses::UI;
           my $cui = new Curses::UI(-color_support => 1, -compat => 0, -utf8 => 1);
           my $win = $cui->add('window_id', 'Window');

           my $textentry = $win->add(
               'mytextentry', 'TextEntry'
           );

           $textentry->focus();
           my $text = $textentry->get();

#print "TEXT: $text \n";
