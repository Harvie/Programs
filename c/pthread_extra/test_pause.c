#include <signal.h>
#include <pthread.h>
#include <pthread_extra.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

void *thread_test(void *arg) {
	//Whole process dies if you kill thread immediately before it is pausable
	//pthread_pause_enable();
	while(1) {
		usleep(1000*300);
		printf("Running%s!\n", (char *)arg);
	}
}

int main() {

	pthread_t a, b;
	pthread_pause_enable(); //Will get inherited by all threads from now on
	//That way you can be sure it is pausable immediately
	pthread_create(&a, NULL, thread_test, " A");
	pthread_create(&b, NULL, thread_test, " B");

	while(1) {
		pthread_pause(b);
		pthread_unpause(a);
		printf("SWITCH A:\n");
		sleep(2);

		printf("SWITCH B:\n");
		pthread_pause(a);
		pthread_unpause(b);
		sleep(2);

		printf("SWITCH A+B:\n");
		pthread_unpause(a);
		pthread_unpause(b);
		sleep(1);
	}

	pthread_join(a, NULL);
	pthread_join(b, NULL);
	printf("DIEDED!\n");
}
