/*
First run tutorial.glade through gtk-builder-convert with this command:
  gtk-builder-convert tutorial.glade tutorial.xml

Then save this file as main.c and compile it using this command
(those are backticks, not single quotes):
  gcc -Wall -g -o tutorial main.c `pkg-config --cflags --libs gtk+-2.0` -export-dynamic

Then execute it using:
  ./tutorial
*/

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

GtkBuilder      *builder;
GtkWidget       *window, *button1, *entry1;


void on_window_destroy (GtkObject *object, gpointer user_data)
{
    gtk_main_quit ();
}

void on_button1_clicked() {
  puts("click!");
  puts(gtk_entry_get_text(entry1));
  //gtk_button_set_label(button1, "lol");
  gtk_button_set_label(button1, gtk_entry_get_text(entry1));
}

int main (int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "test.xml", NULL);
    window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));

    button1 = GTK_WIDGET (gtk_builder_get_object (builder, "button1"));
    entry1 = GTK_WIDGET (gtk_builder_get_object (builder, "entry1"));


   gtk_builder_connect_signals (builder, NULL);

    g_object_unref (G_OBJECT (builder));

    gtk_widget_show (window);
    gtk_main ();

    return 0;
}

