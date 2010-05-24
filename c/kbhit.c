#include <stdio.h>
#include <curses.h>

int main(void) {
	//atexit((void *) endwin);
	//initscr();

	sleep(3);
	while(1) {
		printf("%d,", getc(stdin));
	}
}
