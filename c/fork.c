#include <stdio.h>
#include <stdlib.h>

int test = 1;
int main(void) {
	int *ptr;
	ptr = &test;
	if(!fork()) {
		*ptr = 2; 
		return;
	}
	while(test == 1) printf("");
	printf("lol\n");
}
