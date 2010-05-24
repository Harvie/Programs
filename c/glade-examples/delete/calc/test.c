/*
First run tutorial.glade through gtk-builder-convert with this command:
  gtk-builder-convert tutorial.glade tutorial.xml

Then save this file as main.c and compile it using this command
(those are backticks, not single quotes):
  gcc -Wall -g -o tutorial main.c `pkg-config --cflags --libs gtk+-2.0` -export-dynamic

Then execute it using:
  ./tutorial
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gtk/gtk.h>

GtkBuilder      *builder;
GtkWidget       *window, *entry_a, *entry_b, *entry_calc, *entry_mem, *label_op;
char		tempbuf[128], *ptempchar, nextop;
int		calculated;

void on_button_no_clicked(GtkWidget *widget, gpointer user_data) {
	gtk_entry_append_text(entry_calc, gtk_button_get_label(widget));
}

void calculate() {
	double a, b, e;
	if(calculated) return;
        gtk_entry_set_text(entry_b, gtk_entry_get_text(entry_calc));
	gtk_entry_set_text(entry_calc, "");

	a = strtod(gtk_entry_get_text(entry_a), NULL);
	b = strtod(gtk_entry_get_text(entry_b), NULL);

	switch(nextop) {
		case '+': e = a + b; break;
		case '-': e = a - b; break;
		case '*': e = a * b; break;
		case '/': e = a / b; break;
		case '^': e = pow(a,  b); break;
		case 'S': e = a + b; break;
	}
	sprintf(tempbuf, "%lf", e);
	//printf("%f %c %f = %f\n", a, nextop, b, e);
	gtk_entry_set_text(entry_calc, tempbuf);
	calculated = 1;
}

void on_button_op_clicked(GtkWidget *widget, gpointer user_data) {
	char *oplbl = gtk_button_get_label(widget);

	switch(oplbl[0]) {
		case '+': case '-': case '*': case '/': case '^': case 'S':
			if(!calculated) calculate();
			gtk_entry_set_text(entry_a, gtk_entry_get_text(entry_calc));
			gtk_entry_set_text(entry_calc, "");
			gtk_entry_set_text(entry_b, "");
			gtk_label_set_label(label_op, oplbl);
			nextop = oplbl[0];
			calculated = 0;
		break;

		case '=':
			if(!calculated) calculate();
			gtk_entry_set_text(entry_calc, gtk_entry_get_text(entry_calc));
			calculated = 1;
		break;
	}
}

void on_button_mem_clicked(GtkWidget *widget, gpointer user_data) {
	char *oplbl = gtk_button_get_label(widget);
	double e = 0, m = 0;
	e = strtod(gtk_entry_get_text(entry_calc), NULL);
	m = strtod(gtk_entry_get_text(entry_mem), NULL);
	switch(oplbl[0]) {
		case '+': m += e; break;
		case '-': m -= e; break;
		case 'C': m = 0; break;
	}
	sprintf(tempbuf, "%lf", m);
	gtk_entry_set_text(entry_mem, tempbuf);

}

void on_button_clear_clicked() {
	/*ptempchar = gtk_entry_get_text(entry_calc);
	ptempchar[strlen(ptempchar)-1] = '\x00';
	gtk_entry_set_text(entry_calc, ptempchar);*/
	gtk_entry_append_text(entry_calc, "\r");
}

void reset() {
	calculated = 1;
	gtk_label_set_label(label_op, "?");
	gtk_entry_set_text(entry_calc, "");
	gtk_entry_set_text(entry_a, "");
	gtk_entry_set_text(entry_b, "");
}

void on_window_destroy (GtkObject *object, gpointer user_data)
{
    gtk_main_quit ();
}

int main (int argc, char *argv[])
{
    	gtk_init (&argc, &argv);

    	builder = gtk_builder_new ();
    	gtk_builder_add_from_file (builder, "calc.xml", NULL);
    	window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));

	entry_a = GTK_WIDGET (gtk_builder_get_object (builder, "entry_a"));
    	entry_b = GTK_WIDGET (gtk_builder_get_object (builder, "entry_b"));
    	entry_calc = GTK_WIDGET (gtk_builder_get_object (builder, "entry_calc"));
    	entry_mem = GTK_WIDGET (gtk_builder_get_object (builder, "entry_mem"));
    	label_op = GTK_WIDGET (gtk_builder_get_object (builder, "label_op"));

   	gtk_builder_connect_signals (builder, NULL);
    	g_object_unref (G_OBJECT (builder));

	reset();
	gtk_widget_show (window);
   	gtk_main ();

	return 0;
}

