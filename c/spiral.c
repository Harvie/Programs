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

//config
#define HOUR_NUMBERS 1
#define HI_PRECISION 1
#define SHOW_DIGITS 1
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
void render_clock(double a) {
	//colors
	char color_empty = ' ';
	char color_s = '*';
	char color_center = 'o';

	//get size of terminal
	static int lines = -1;
	static int cols = -1;
	if(lines < 7 || cols < 7) {
		FILE *p;
		p = popen("stty size", "r");
		fscanf(p, "%d %d", &lines, &cols);
		close((int)p);
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

	//draw seconds hand
	//phi = -90+(s*(360/60));
	//int i;
	for(i=0;i<360;i+=28) {
		double inc=0;
		for(r=0;r<(size/2);r+=inc) {
			x = round((size/2+polar2x(a+i,r))*zoom);
			y = round(size/2+polar2y(a+i,r));
			cifernik[x][y] = color_s;
			a-=5;
			inc+=0.009;
		}
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
	printf("\033[0;0H");
	for(y=0;y<size;y++) {
		for(r=0;r<=((cols-(size*zoom))/2);r++) putchar(' ');
		for(x=0;x<((size*zoom)-(zoom-1));x++) {
			putchar(cifernik[x][y]);
		}
		if(y!=(size-1)) putchar('\n');
	}
}

int main(void) {
	int a = 0;

	while(1) {
			render_clock(a=(a+5)%360);
			usleep(100000);
	}
}
