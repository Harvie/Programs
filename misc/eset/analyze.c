#include <stdio.h>

int main(int argc, char *argv[]) {
	char filename[] = "test";
	char *data[840]; //lines
	char line[29700]; //max line lenght

	FILE *fp;

	fp = fopen(filename, "rw");
	while(!feof(fp)) {
		fgets(line, 29698, fp);
		//long int strtol(const char *nptr, char **endptr, int base);

	}

}
