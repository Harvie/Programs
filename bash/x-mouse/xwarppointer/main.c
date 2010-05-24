#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void
usage(const char *err, ...)
{
    va_list va;
    va_start(va, err);

    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, err, va);

    va_end(va);

    fprintf(stderr, "\n"
"usage: xwarppointer <command> <args>\n\n"
" commands:\n"
"   screen <+/-> | <screen #>       Move cursor to specific screen, or        \n"
"                                   relative to current one. Does not change  \n"
"                                   the cursor's position on the screen.      \n"
"\n"
"   abspos <x|.> <y|.>              Moves cursor to given X and Y coordinates.\n"
"                                   If . is specified instead of a #, it      \n"
"                                   doesn't change that axis.                 \n"
"\n"
"   relpos <x offset> <y offset>    Move the cursor relative to its current   \n"
"                                   position.                                 \n"
"\n"
"   get                             Return the cursors position.              \n"
"\n");
    exit(1);
}

int
myatoi(const char *str, int *ret) {
    char *p;
    int i;

    i = strtod(str, &p);
    if (p == str)
	return 0;
    *ret = i;
    return 1;
}

int
main(int argc, char *argv[])
{
    Display *display;
    Window root;
    
    if ((display = XOpenDisplay(NULL)) == NULL) {
	fprintf(stderr, "Cannot open local X-display.\n");
	exit(1);
    }
    root = DefaultRootWindow(display);

    if (argc < 2) usage("wrong number of args");

    if (!strcasecmp(argv[1], "screen")) {
	Window fromroot, tmpwin;
	XWindowAttributes attr;
	int screen = -1, rot, x, y, tmp;

	if (argc != 3)
	    usage("screen: wrong number of args");

	if (*argv[2] == '+')
	    rot = 1;
	else if (*argv[2] == '-')
	    rot = -1;
	else
	    screen = atoi(argv[2]);

	XQueryPointer(display, root, &fromroot, &tmpwin, &x, &y, &tmp, &tmp, &tmp);

	if (screen == -1) {
	    XGetWindowAttributes(display, fromroot, &attr);
	    screen = XScreenNumberOfScreen(attr.screen);

	    screen += rot;
	    if (screen >= ScreenCount(display))
		screen -= ScreenCount(display);
	    if (screen < 0)
		screen += ScreenCount(display);

	} else {
	    if (screen >= ScreenCount(display) || screen < 0) {
		fprintf(stderr, "Cannot get root window of screen!\n");
		exit(1);
	    }
	}

	root = RootWindow(display, screen);
	XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
	XFlush(display);

    } else if (!strcasecmp(argv[1], "abspos")) {
	int x, y, tmp;
	Window fromroot, tmpwin;

	if (argc != 4)
	    usage("abspos: wrong number of args");

	XQueryPointer(display, root, &fromroot, &tmpwin, &x, &y, &tmp, &tmp, &tmp);

	if (argv[2][0] != '.')
	    if (!myatoi(argv[2], &x))
		usage("abspos: invalid x pos: %s", argv[2]);
	if (argv[3][0] != '.')
	    if (!myatoi(argv[3], &y))
		usage("abspos: invalid y pos: %s", argv[3]);

	XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
	XFlush(display);

    } else if (!strcasecmp(argv[1], "relpos")) {
	int x, y, tmp, x2, y2;
	Window fromroot, tmpwin;

	if (argc != 4)
	    usage("relpos: wrong number of args");

	XQueryPointer(display, root, &fromroot, &tmpwin, &x, &y, &tmp, &tmp, &tmp);

	if (!myatoi(argv[2], &x2)) usage("relpos: invalid x pos: %s", argv[2]);
	if (!myatoi(argv[3], &y2)) usage("relpos: invalid y pos: %s", argv[3]);
	x += x2;
	y += y2;

	XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
	XFlush(display);

    } else if (!strcasecmp(argv[1], "get")) {
	int x, y, tmp;
	Window fromroot, tmpwin;

	if (argc != 2)
	    usage("get: wrong number of args");

	XQueryPointer(display, root, &fromroot, &tmpwin, &x, &y, &tmp, &tmp, &tmp);

	printf("%d %d\n", x, y);

    } else {
	usage("invalid command: %s", argv[1]);
    }

    exit(0);
}
