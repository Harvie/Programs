#!/usr/bin/php -c/etc/php/php-gtk.ini
#GTK Hello world! by Harvie 2oo9

<?php

if (!class_exists('gtk')) {
    die("Please load the php-gtk2 module in your php.ini\r\n");
}
 
$wnd = new GtkWindow();
$wnd->set_title('Hello world');
$wnd->connect_simple('destroy', array('gtk', 'main_quit'));
 
$lblHello = new GtkLabel("Just wanted to say\r\n'Hello world!'");
$wnd->add($lblHello);
 
$wnd->show_all();
Gtk::main();
?>
