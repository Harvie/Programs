#include <stdio.h>
#include <pthread.h>

void *go(void *args) {
	while(1) {
		puts("lol!");
	}
}

int main() {
	pthread_t t;
	//pthread_create(&t, NULL, go, NULL);
	pthread_create(&t, NULL, puts, "hello!");
	sleep(1);
}
