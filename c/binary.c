#include <stdio.h>

typedef struct {
	unsigned a:1;
	unsigned b:1;
	unsigned c:1;
	unsigned d:1;
	unsigned e:1;
	unsigned f:1;
	unsigned g:1;
} bbyte;

int main() {
	unsigned char c = 10;
	struct bbyte *bb;
	bb = (bbyte *) &c;
	//printf("%d%d%d%d%d%d%d%d",)
	printf("%d\n", bb->a);
}
