#!/usr/bin/env perl

use UI::Dialog;
         my $d = new UI::Dialog ( backtitle => 'backtitle', title => 'title',
                                  #height => 20, width => 65 , listheight => 5,
                                  order => [ 'CDialog', 'WhipTail', 'ASCII', 'Zenity', 'XDialog' ] );

         $d->msgbox( title => 'Welcome!', text => 'Welcome one and all!' );
					my $text = $d->fselect( path => '/tmp' );

