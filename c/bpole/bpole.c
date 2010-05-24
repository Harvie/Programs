#include <stdio.h>

//deklarace bitového pole
struct *bits {
	unsigned int a: 1;
	unsigned int b: 1;
	unsigned int c: 1;
	unsigned int d: 1;
	unsigned int e: 1;
	unsigned int f: 1;
	unsigned int g: 1;
	unsigned int h: 1;
};
//struct lol *bits;
char byt;

int main(){
	while(1){
		byt = getchar();
		bits = &byt;
		printf("%d:%d:%d:%d:%d:%d:%d:%d\n", bits->a,	bits->b,	bits->c,	bits->d,	bits->e,	bits->f,	bits->g,	bits->h);
	}
}
