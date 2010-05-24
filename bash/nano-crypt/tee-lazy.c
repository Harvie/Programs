#include <stdio.h>

int main(int argc, char *argv[])
{
	if(argc < 2) {
		printf("Usage: %s output-file < input-file\n", argv[0]);
	}

	FILE *outfp;
	outfp = 0;
	unsigned int c;
	while( (c = getchar()) != EOF ) {
		if(!outfp) outfp = fopen(argv[1], "wb");
		putc(c, outfp);
  }
	fclose(outfp);
  return 0;
}
