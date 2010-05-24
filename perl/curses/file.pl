#!/usr/bin/env perl

           use Curses::UI;
           my $cui = new Curses::UI( -color_support => 1, -compat => 0 );
           #my $win = $cui->add('window_id', 'Window');

           my $file = $cui->filebrowser(
               -path => "/tmp",
               -show_hidden => 1,
           );

           # Filebrowser will return undef
           # if no file was selected.
           if (defined $file) {
               unless (open F, ">$file") {
                   print F "Hello, world!\n";
                   close F;
           } else {
               $cui->error(qq(Error on writing to "$file":\n$!));
           }
					}

#print "TEXT: $text \n";
