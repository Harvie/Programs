/*
pracuje se na tom...

compile with
mcs hello.cs -pkg:gtk-sharp-2.0
mono hello.exe
*/

using Gtk;
using System;

class Hello {

	static void Main()
  {
  	Application.Init ();

		GTKBuilder builder = new GtkBuilder();

		builder->add_from_file("hello.ui");
		builder->connect_signals();

                Window window = new Window ("helloworld");
                window.Show();

                Application.Run ();

	}
}

