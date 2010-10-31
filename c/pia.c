#include <stdio.h>
#include <math.h>
#include <time.h>
#define PI 3.141592653589793
#include <termios.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  //get size of terminal
  int lines = 20;
  int cols = 20;
  /*FILE *p;
  p = popen("stty size", "r");
  fscanf(p, "%d %d", &lines, &cols);
  close((int)p);*/
  struct winsize ws;
  if ((ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 &&
    ioctl(STDERR_FILENO, TIOCGWINSZ, &ws) == -1 &&
    ioctl(STDIN_FILENO,  TIOCGWINSZ, &ws) == -1) ||
    ws.ws_col == 0) {
      lines = 25;
      cols = 80;
  } else {
    lines = ws.ws_row;
    cols = ws.ws_col;
  }

	char *format;
	asprintf(&format,"%%0.%dlf\r",cols-2);


  double pi=0, i=1;

	while(1) {
		printf(format, pi);
		pi+=(4/i);
		i+=2;
		pi-=(4/i);
		i+=2;
	}
  //system("PAUSE");
  //return EXIT_SUCCESS;
}
