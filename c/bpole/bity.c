#include <stdio.h>

int main() {
	char c, d, i;
	while(1) {
		c = getchar();
		 for(i=0;i<8;i++) {
		   d = c && '\x01';
		   printf("%d ", d);
		   c = c << 1;
		 }
	}
}
