/* GTK Hello world! by Harvie 2oo9
 *
 * Compile this with:
 * gcc -o hello hello.c $(pkg-config --cflags --libs gtk+-2.0) -export-dynamic
 * or
 * gcc -Wall -g -o hello hello.c `pkg-config --cflags --libs gtk+-2.0` -export-dynamic
 */

#include <stdio.h>
//#include <string.h>
#include <gtk/gtk.h>

GtkBuilder      *builder;
GtkWidget       *window1, *button1, *entry1;

void on_window1_destroy (GtkObject *object, gpointer user_data)
{
	gtk_main_quit();
}

void on_button1_clicked() {
  gtk_button_set_label(button1, gtk_entry_get_text(entry1));
  puts(gtk_entry_get_text(entry1));
}

int main (int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "hello.ui", NULL);

  window1 = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
  button1 = GTK_WIDGET (gtk_builder_get_object (builder, "button1"));
  entry1 = GTK_WIDGET (gtk_builder_get_object (builder, "entry1"));

	gtk_builder_connect_signals(builder, NULL);

  g_object_unref(G_OBJECT (builder));

  gtk_widget_show(window1);
  gtk_main();

  return 0;
}

