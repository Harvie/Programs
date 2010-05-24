#include <stdio.h>

int main() {
	char inbuf[3];
  FILE *mice = fopen("/dev/input/mice", "rb");
  if(!mice) exit(1);
  
  while(1) {
  	printf(" %b", getc(mice));
  	printf(" %d", getc(mice));
  	printf(" %d\n", getc(mice));
  }
	                                                        
}
