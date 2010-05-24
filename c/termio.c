#include <stdio.h>
#include <stdlib.h>

int main() {
	unsigned char *lol;
	lol = malloc(99*sizeof(char));
	*lol = "HAI SUXORZ!!!!\n";
	free(lol);
	printf("%s", lol);
}
