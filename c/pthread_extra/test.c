#include <pthread.h>
#include <pthread_extra.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>


int main_mumu() {
	//Prepare mutex array for tests
	static pthread_mutex_t la = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t lb = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t lc = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t ld = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t *lck[4] = {&la, &lb, &lc, &ld};

	//Set timeout
	struct  timespec tm;
	clock_gettime(CLOCK_REALTIME, &tm);
	tm.tv_sec  += 5;

	//Lock one of the locks for testing
	pthread_mutex_lock(lck[2]);

	if(!pthread_mutex_timedlock_multi(lck, 4, &tm)) {
	//if(!pthread_mutex_timedlock_two(&la, lck[2], &tm)) {
		printf("LOCKED!\n");
	} else {
		printf("FAILED!\n");
	}

}


pthread_mq_t myq;

void *thread_recv(void *args) {
	char str[128];
	while(1) {
		pthread_mq_receive_generic(&myq, &str, false, PTHREAD_XTIME_FOREVER);
		printf("RECVD: %.6s\t\t(waiting %d)\n", str, pthread_mq_waiting(&myq));
		//sleep(1);
	}
}

int main() {
	main_mumu();

	char tmp[128];

	pthread_mq_init(&myq, 6, 5);

	pthread_t t;
	pthread_create(&t, NULL, thread_recv, NULL);

	pthread_mq_send_generic(&myq, "AHOJ1", false, NULL);
	pthread_mq_send_generic(&myq, "AHOJ2", false, NULL);
	pthread_mq_send_generic(&myq, "AHOJ3", true, NULL);
	pthread_mq_send_generic(&myq, "AHOJ4", true, NULL);
	pthread_mq_send_generic(&myq, "AHOJ5", false, NULL);
	pthread_mq_send_generic(&myq, "AHOJ6", true, NULL);

	while(1) {
		pthread_mq_send_generic(&myq, "B", false, NULL);
		pthread_mq_send_generic(&myq, "A", true, NULL);
		pthread_mq_send_generic(&myq, " A", false, NULL);
		pthread_mq_send_generic(&myq, " B", false, NULL);
		sleep(1);
	}

	pthread_join(t, NULL);
}

