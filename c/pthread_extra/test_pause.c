#define PTHREAD_FREEWHEEL

#include <signal.h>
#include <pthread.h>
#include <pthread_extra.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

//#define printf(...) (0)

pthread_t main_thread;

void *thread_test(void *arg) {
	//Whole process dies if you kill thread immediately before it is pausable
	//pthread_pause_enable();
	while(1) {
		pthread_nsleep(0, 1000*1000*300);
		pthread_pause_all();
		printf("Running%s!\n", (char *)arg);
		pthread_unpause_all();
		pthread_pause(main_thread);
		pthread_unpause(main_thread);
	}
}

int main() {

	main_thread = pthread_self();

	pthread_t a, b;
	pthread_pause_enable(); //Will get inherited by all threads from now on
	//That way you can be sure it is pausable immediately
	pthread_extra_create(&a, NULL, thread_test, " A");
	pthread_extra_create(&b, NULL, thread_test, " B");
	//pthread_sleep(1);

	//printf("OK\n");
	/*
	for(int32_t i = 1;i>0;i++) {
		pthread_pause(a);
		pthread_pause(a);
		pthread_unpause(a);
		pthread_unpause(a);
	}
	*/
	/*
	exit(23);
	pthread_pause(a);
	pthread_unpause(b);
	*/

	while(1) {
		pthread_pause(b);
		pthread_unpause(a);
		pthread_pause_disable();
		pthread_pause_disable();
		pthread_pause_disable();
		printf("SWITCH A:\n");
		pthread_pause_enable();
		pthread_pause_enable();
		pthread_pause_enable();
		pthread_sleep(2);

		pthread_pause_all();
		printf("SWITCH B:\n");
		pthread_unpause_all();
		pthread_pause(a);
		pthread_unpause(b);
		pthread_sleep(2);

		//pthread_pause_disable();
		pthread_pause_all();
		printf("SWITCH A+B:\n");
		pthread_unpause_all();
		//pthread_pause_enable();
		pthread_unpause(a);
		pthread_unpause(b);
		pthread_sleep(1);

		//pthread_pause_disable();
		pthread_pause_all();
		printf("SWITCH MAIN ONLY:\n");
		//pthread_pause_enable();
		pthread_pause_all();
		//printf("\n");
		pthread_sleep(1);
		printf("SWITCH MAIN A+B:\n");
		pthread_unpause_all();
		//printf("\n");
		pthread_sleep(1);
	}

	pthread_join(a, NULL);
	pthread_join(b, NULL);
	printf("DIEDED!\n");
}
