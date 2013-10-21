#include <stdio.h>

#define SAMPLE_MIN -128
#define SAMPLE_MAX 127

int main() {
	unsigned long count=0;
	char sample, sample_last=SAMPLE_MIN, x=0, y=0;
	while(!feof(stdin)) {
		fread(&sample,1,1,stdin);
		if(
			(sample_last < 0 && sample > SAMPLE_MAX/2)
			|| (sample_last >= 0 && sample < SAMPLE_MIN/2)
		)
		{
			putchar('.');
			//printf("\r%lu    ", ++count);
			fflush(stdout);
			sample_last=sample;
		}
	}
}
