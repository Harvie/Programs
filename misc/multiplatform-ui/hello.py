#!/usr/bin/env python
#GTK Hello world! by Harvie 2oo9

import gtk
 
class Hello:
 
	def on_window1_destroy(self, widget, data=None): 
		gtk.main_quit()

	def on_button1_clicked(self, widget, data=None):
		self.button1.set_label(self.entry1.get_text())
		print(self.entry1.get_text())

	def __init__(self):   
		builder = gtk.Builder()
		builder.add_from_file("hello.ui")
  
		self.window1 = builder.get_object("window1")
		self.button1 = builder.get_object("button1")
		self.entry1 = builder.get_object("entry1")
		builder.connect_signals(self)

		self.window1.show()
   
if __name__ == "__main__":
	main = Hello()
	gtk.main()
