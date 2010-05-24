#include <stdio.h>

int main() {
	/*asm("jmp poly_cont\n\t"
			"incl %eax\n\t"	"incl %ebx\n\t"	"incl %ecx\n\t"	"incl %edx\n\t"
			"decl %eax\n\t"	"decl %ebx\n\t"	"decl %ecx\n\t"	"decl %edx\n\t"
			"poly_cont:\n\t"); //"@CABHKIJ" */
	//asm(".rept 20 nop \n\t .endr");
	puts("KALK\n");
	
	float a, b;
	char op;
	char loop = 1;

	while(loop) {
		scanf("%f", &a);
		scanf("%c\n", &op);
		scanf("%f", &b);

		printf("%f %c %f = ?\n", a, op, b);

	}
}
