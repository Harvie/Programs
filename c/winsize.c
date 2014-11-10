#include <stdio.h>
#include <sys/ioctl.h>

static int get_win_size (int fd, struct winsize *win)
{
  return ioctl (fd, TIOCGWINSZ, (char *) win);
}

int main(void) {
	struct winsize win;
	if(!get_win_size(1, &win)) puts("Chyba!");
	printf("%d %d :-)\n", win.ws_row, win.ws_col);
}
