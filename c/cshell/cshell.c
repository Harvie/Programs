#include <stdio.h>
#include <dlfcn.h>

typedef int (*func)(int, int);
func add, sub, mul, div;

int main()
{
	int a = 5, b = 4;
	void *arithHandle = NULL;

	arithHandle = dlopen("libarithmetic.so", RTLD_NOW);
	if ( NULL == arithHandle )
	{
		printf("Arithmetic library cant be opened\n");
	}

	add = (func) dlsym (arithHandle, "add");
	sub = (func) dlsym (arithHandle, "sub");
	mul = (func) dlsym (arithHandle, "mul");
	div = (func) dlsym (arithHandle, "div");

	printf("Add :: %d\n", add(a, b));
	printf("Sub :: %d\n", sub(a, b));
	printf("Mul :: %d\n", mul(a, b));
	printf("Div :: %d\n", div(a, b));

	dlclose(arithHandle); //Its important to call dlclose() when you are done
	return 0;
}
