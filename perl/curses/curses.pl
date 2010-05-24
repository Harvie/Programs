#!/usr/bin/env perl
#pacman -S perl-curses-ui

use utf8;

use strict;
use warnings;
use Curses::UI;

#use I18N::Collate;
#setlocale('LC_COLLATE', 'cs_CZ.utf8');
#use encoding;

#binmode(STDOUT, ":crlf");
#binmode(STDIN, ":crlf");

#binmode(STDOUT, ":encoding(utf8)");
#binmode(STDIN, ":encoding(utf8)");




my $cui = new Curses::UI( -color_support => 1, -compat => 0 );

#Create a menu ^

        my @menu = (
          { -label => 'File', 
            -submenu => [
           { -label => 'Exit      ^Q', -value => \&exit_dialog  }
                        ]
           },
        );

#In order to describe the structure of a menu Curses::UI uses a rather ugly construct out of hash and arrayrefs. See Curses::UI::Menubar for details. What you do at this point is to create a Menubar with just one entry and one submenu. The entry is 'File' and the submenu is 'Exit'. The value of this menu item is a reference to a sub called exit_dialog.
#Dialogs ^

        sub exit_dialog()
        {
                my $return = $cui->dialog(
                        -message   => "Do you really want to quit?",
                        -title     => "Are you sure???", 
                        -buttons   => ['yes', 'no'],

                );

        				exit(0) if $return;
        }

#The dialog method of Curses::UI gives us an easy and convenient way to create dialogs on the main screen. A dialog is a way to interact with the user in order to ask him a question or give him important information. This dialog is a more complex one, which asks the question whether or not you really want to exit. As the button for "yes" would return us a true value, you can easily exit on this return value.
#Add the Menubar ^

        my $menu = $cui->add(
                'menu','Menubar', 
                -menu => \@menu,
                -fg  => "blue",
        );

#To finally add the Menubar to our root object, you have to call the add method on the Curses UI object. You specify the internal name of the widget as the first argument, the widget type as the second argument (like Label, TextViewer, etc.) and the menu structure you created at the beginning as an array reference as third object. Because you want the Menubar to have a blue theme, you give him the -fg option "blue". There are a couple of colors you can use, see Curses::UI::Color for details.
#Add a window ^

        my $win1 = $cui->add(
                             'win1', 'Window',
                             -border => 1,
                             -y    => 1,
                             -bfg  => 'red',
                     );

#There are only two types of object you can add to the Curses::UI root object: Menubars and Windows. All other widgets have to be inserted into a window. Of course you can add a Menubar to a window, but not vice versa ;-). The add method always has the same two first arguments: the internal name and the widget type. The internal name can be used to find an object. The method getobj takes this name and returns us the corresponding object out of the hierarchy. See Curses::UI for details. Again you want some fancy colors, so you tell the window to have a border, leave some space for the Menubar (-y => 1) and set the border foreground color to red.
#Add a widget ^

        my $texteditor = $win1->add("text", "TextEditor",
                                 -text => "Here is some text ěščřžýáíé\n"
                                        . "And some more");

#The next step is to add a useful widget to our new small Curses::UI app. Here you take a TextEditor widget which performs basic tasks as a text editor. You add some initial text to the widget to make it not seem that empty.
#Making keybindings ^

        $cui->set_binding(sub {$menu->focus()}, "\cX");
        $cui->set_binding( \&exit_dialog , "\cQ");

#You want to be able to focus the Menubar if you finished editing in the TextEditor widget. Therefore you set a binding to the focus function of the menu and the key sequence Control (specified by \c) combined with X. Now you can easily return to the menu after editing. Because it is easier to have a shortcut for closing the application you add a binding for the sequence Control-Q to our nice exit_dialog method.
#The final steps ^

        $texteditor->focus();
        $cui->mainloop();
