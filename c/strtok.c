#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char text[] = "Hello \ndolly \r\nomg!\tHow are you?\n  \n\t";
	puts(text);
	puts("----------------------------");

	char *str;
	char delim[] = " \r\n\t";
	for(str = strtok(text, delim);str != NULL;str = strtok(NULL, delim)) {
		printf("[%s]\n", str);
	}
}
