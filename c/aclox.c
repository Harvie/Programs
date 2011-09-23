/*
 * Compile with:
 * gcc -lm aclox.c -o aclox; ./aclox
 * or
 * CFLAGS='-lm' make aclox; ./aclox
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include <termios.h>
#include <sys/ioctl.h>

//config
#define HOUR_NUMBERS 1
#define HOUR_HEXADECIMAL 1
#define HI_PRECISION 1
#define SHOW_DIGITS 1
#define DIGITS_HEXADECIMAL 1
#define MINUTE_DOTS 1
#define SMOOTH_MOTION 1
#define zoom 2

//useful macros
#define max(a,b) ((a)>=(b)?(a):(b))
#define min(a,b) ((a)<=(b)?(a):(b))
#define PI 3.141592654
#define deg2rad(a) (((a)/180)*PI)
//phi = angle, r = length
#define polar2x(phi, r) ((r) * cos(deg2rad(phi)))
#define polar2y(phi, r) ((r) * sin(deg2rad(phi)))
#define cls() printf("\033[2J")
#define top() printf("\033[0;0H")

int llines = -1, lcols = -1;
void render_clock(double h, double m, double s) {
	//colors
	char color_empty = ' ';
	char color_center = '+';
	char color_number = '#';
	char color_dot = '.';
	char color_h = 'H';
	char color_m = 'm';
	char color_s = '*';

	//convert digital time to "analog" time (enables smooth motion)
	if(SMOOTH_MOTION) {
		m += s/(double)60;
		h += m/(double)60;
	}

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

	//calculate size
	int size = 40;
	size = min(lines,cols/zoom);
	size = ((size%2)==0?size-1:size); //make size odd
	//printf("%d\n", size);

	//empty canvas
	char cifernik[size*zoom][size];
	int i, x, y;
	double phi, r;
	for(y=0;y<size;y++) for(x=0;x<(size*zoom);x++) cifernik[x][y] = color_empty;

	//draw minute dots
	if(MINUTE_DOTS && size > 40) for(i=1;i<=60;i++) {
		phi = -90+(i*(360/60));
		r = size/2;
		x = round((r+polar2x(phi,r))*zoom);
		y = round(r+polar2y(phi,r));
		cifernik[x][y] = color_dot;
	}

	//draw numbers
	for(i=1;i<=12;i++) {
		phi = -90+(i*(360/12));
		r = size/2;
		x = round((r+polar2x(phi,r))*zoom);
		y = round(r+polar2y(phi,r));
		if(HOUR_NUMBERS) {
			cifernik[x][y] = '0'+(i%10);
			if(HOUR_HEXADECIMAL) {
				sprintf(&cifernik[x][y],"%X",i);
			} else {
				if(i>9) cifernik[x-1][y] = '0'+(i/10);
			}
		} else {
			cifernik[x][y] = color_number;
		}
	}

	//draw minute hand
	phi = -90+(m*(360/60));
	for(r=0;r<(size/2);r++) {
		x = round((size/2+polar2x(phi,r))*zoom);
		y = round(size/2+polar2y(phi,r));
		cifernik[x][y] = color_m;
	}

	//draw hour hand
	phi = -90+(h*(double)(360/12));
	for(r=0;r<(size/3);r++) {
		x = round((size/2+polar2x(phi,r))*zoom);
		y = round(size/2+polar2y(phi,r));
		cifernik[x][y] = color_h;
	}

	//draw seconds hand
	phi = -90+(s*(360/60));
	for(r=0;r<(size/2);r++) {
		x = round((size/2+polar2x(phi,r))*zoom);
		y = round(size/2+polar2y(phi,r));
		cifernik[x][y] = color_s;
	}

	//draw center dot
	cifernik[(size/2)*zoom][size/2] = color_center;

	//cls when terminal size changes
	if(lines != llines || cols != lcols) {
		cls();
		llines=lines;
		lcols=cols;
	}

	//print to terminal
	for(y=0;y<size;y++) {
		for(r=0;r<=((cols-(size*zoom))/2);r++) putchar(' ');
		for(x=0;x<((size*zoom)-(zoom-1));x++) {
			putchar(cifernik[x][y]);
		}
		if(y!=(size-1)) putchar('\n');
	}
}

int main(void) {
	int s = -1;
	time_t cast;
	struct tm *cas;

	while(1) {
		cast = time(NULL);
		cas = localtime(&cast);

		if(cas->tm_sec != s){
			s = cas->tm_sec;
			top();
			render_clock(cas->tm_hour,cas->tm_min,cas->tm_sec); //analog
			if(SHOW_DIGITS || DIGITS_HEXADECIMAL) putchar('\r');
			if(SHOW_DIGITS) printf("DEC:[%.2d:%.2d:%.2d]",cas->tm_hour,cas->tm_min,cas->tm_sec); //digital
			if(SHOW_DIGITS || DIGITS_HEXADECIMAL) putchar(' ');
			if(DIGITS_HEXADECIMAL) printf("HEX:[%.2x:%.2x:%.2x]",cas->tm_hour,cas->tm_min,cas->tm_sec); //digital hexadec
		}
		if(HI_PRECISION) { usleep(100000); }
		else { sleep(1); }
	}
}
