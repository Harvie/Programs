#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	int g, r, max = 10;
	char buff[80];
	while(1) {
		r = rand() % max;
		printf("hadej cislo (od 0 do"
			"%d):", max - 1);
		fgets(buff, sizeof(buff)-2, stdin);
		*strpbrk(buff, "\r\n") = '\0';
		if (!*buff)
			break;
		g = strtol(buff, NULL, 10);
		if(r == g) puts("Bravo maestro!");
		else printf("Loooooooozre! "
			"bylo to %d\n", r);
		
	}
	puts("konec!");
}
