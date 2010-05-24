#!/usr/bin/php -c/etc/php/php-gtk.ini
<?php
//GTK Hello world! by Harvie 2oo9

$app = new HelloWorld();
Gtk::main();

class HelloWorld {

	public $widgets;
	public $builder;

	function __construct() {
		$this->builder = $builder = new GtkBuilder();
		
		$builder->add_from_file('hello.ui');
		$builder->connect_signals_instance($this);

		/*
		$objects = $builder->get_objects();
		echo count($objects), " objects were created by GtkBuilder\n";

		$builder->set_translation_domain('myapp');
		echo $builder->get_translation_domain(), "\n";

		var_dump($builder->get_type_from_name('GtkButton'));
		*/

		foreach(array('window1','entry1','button1') as $widget) {
			$this->widgets[$widget] = $builder->get_object($widget);
		}
		$this->widgets['window1']->show_all();

	}

	function on_button1_clicked()	{
		$this->widgets['button1']->set_label($this->widgets['entry1']->get_text());
		echo( $this->widgets['entry1']->get_text()."\n" );
	}

	function on_window1_destroy(){
		Gtk::main_quit();
		exit();
	}


}
?>
