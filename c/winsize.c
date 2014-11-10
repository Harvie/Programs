/*
 * Prints size of terminal window in characters
 */

#include <stdio.h>
#include <sys/ioctl.h>

static int get_win_size (int fd, struct winsize *win)
{
  return ioctl (fd, TIOCGWINSZ, (char *) win);
}

int main(void) {
	struct winsize win;
	if(!get_win_size(1, &win)) puts("Error!");
	printf("%d %d :-)\n", win.ws_row, win.ws_col);
}
