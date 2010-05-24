#include <stdio.h>
#include <time.h>
#include <math.h>

#define PWSIZE 32
#define TIMERES 1000

int main() {

while(1) printf("c: %d, cps %d\n", clock(), CLOCKS_PER_SEC);

char passwd[PWSIZE], loginpasswd[PWSIZE];
long timestamp[PWSIZE], logintimestamp;
int i, sensitivity = 200;

setbuf(stdin, NULL);
setbuf(stdout, NULL);


printf("Enter new pwd: ");
for(i=0;i<(PWSIZE-1);i++) {
	clock();
	passwd[i] = getchar();
	if(passwd[i] == '\n') {
		i++;
		break;
	}
	putchar('*');
	timestamp[i] = 0;
	timestamp[i] += clock()/CLOCKS_PER_SEC;
}
passwd[i] = 0;

while(1) {

	printf("Enter login pwd: ");
	for(i=0;i<(PWSIZE-1);i++) {
		clock();
		if(getchar() != passwd[i]) {
			printf("\nIncorrect!\n");
			break;
		}
		logintimestamp = 0;
		logintimestamp += clock()/CLOCKS_PER_SEC;
		logintimestamp -= timestamp[i];
		if(sqrt(logintimestamp*logintimestamp) > sensitivity) {
			printf("\nIncorrect timing!\n");
			break;
		}
	}

}



}
