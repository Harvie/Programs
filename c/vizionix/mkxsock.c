#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <gtk/gtk.h>

#define ENV_PREFIX "XSOCK_"

int return_true(GtkWidget *widget, gpointer data) { return 1; }

int switch_sockets(GtkWidget *widget, gpointer data) {
	fprintf(stderr,"SWITCH: %d\n", (int)data);
	return 1;
}

void print_help(char ** argv) {
	printf(
		"Usage: %s [args]\n"
		"Args:\n"
		"\t -c color\n"
		"\t -w width\n"
		"\t -h height\n"
		//"\t -r reuse " ENV_PREFIX "* from environment\n"
		"\t -? (display help)"
		"\nExample: %s -c black -w 640 -h 480\n\n",
		argv[0], argv[0]);
}

gint main(gint argc, gchar ** argv)
{
	gchar *colorstr = "black";
	int width = 640;
	int height = 480;

	int opt;
	while ((opt = getopt(argc, argv, "?c:w:h:")) != -1) {
		switch (opt) {
			case 'c':
				colorstr = optarg;
				break;
			case 'w':
				width = atoi(optarg);
				break;
			case 'h':
				height = atoi(optarg);
				break;
			case 'r': //Not yet
				if(getenv(ENV_PREFIX"PID")
					&& getenv(ENV_PREFIX"WID")
					&& kill(atoi(getenv(ENV_PREFIX"PID")), 0)
				) {
					puts("REUSABLE");
					return 0;
				}
				break;
			case '?':
				print_help(argv);
				return 0;
				break;
		}
	}

	//printf("%s\n", colorstr); return 0;

	gtk_init(&argc, &argv);

	//Colors
	GdkColor color;
	gdk_color_parse(colorstr, &color);

	//Create window
	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *sock[1];
	sock[0] = gtk_socket_new();
	g_signal_connect(sock[0], "plug-added", G_CALLBACK(switch_sockets), (gpointer)23);
	g_signal_connect(sock[0], "plug-removed", G_CALLBACK(return_true), NULL);
	g_signal_connect(win, "delete-event", gtk_main_quit, NULL);
	gtk_widget_set_size_request(sock[0], width, height);
	gtk_widget_modify_bg(sock[0], GTK_STATE_NORMAL, &color);
	gtk_container_add(GTK_CONTAINER(win), sock[0]);
	gtk_widget_show_all(win);

	//Print id
	GdkNativeWindow wid = gtk_socket_get_id(GTK_SOCKET(sock[0]));
	int pid;
	if ((pid = fork())) {
		printf(ENV_PREFIX"WID=%d\n"ENV_PREFIX"PID=%d\n", wid, pid);
		return 0;
	}
	fclose(stdout);

	//Run
	gtk_main();
	return 0;
}
