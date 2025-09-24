#include <stdio.h> //Pouze printf() pro debug
#include <stdlib.h>
#include <unistd.h>

void readline_rec(int offset, int size, char **buffer) {
	char *fragment = malloc(size); //Mohlo by byt i staticky, ale size by musela byt konstantni
	int finished = 0;

	for(int i=0; i<size; i++) {
		size_t got = read(0, &fragment[i], 1);
		if(got <= 0 || fragment[i] == '\n' || fragment[i] == '\r') {
			fragment[i]=0;
			*buffer = malloc(offset + size); //Mohlo by se pouzit +i misto +size, ale muselo by se zohlednit i pri kopirovani
			finished=1;
			break;
		}
	}

	if(!finished) readline_rec(offset+size, size*2, buffer);
	for(int j=0; j<size; j++) (*buffer)[offset+j] = fragment[j];
	free(fragment);
	return;
}


int main() {
	char *out = NULL;
	readline_rec(0, 1, &out);
	printf("Precteno: %s\n", out);
	free(out);
}
