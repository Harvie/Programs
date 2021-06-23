/*
 * CFLAGS=-lpthread make pthread_msgqueue
 */

#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

typedef struct pthread_mq_t {
	pthread_mutex_t lock;
	pthread_cond_t cond_readable;
	pthread_cond_t cond_writable;
	void * data;
	size_t msg_size;
	size_t msg_count;
	size_t msg_count_max;
	size_t head_idx;
	char * name;
} pthread_mq_t;

bool pthread_mq_readable(pthread_mq_t *mq) { return (mq->msg_count > 0); }
bool pthread_mq_writable(pthread_mq_t *mq) { return (mq->msg_count < mq->msg_count_max); }

bool pthread_mq_init(pthread_mq_t *mq, size_t msg_size, size_t msg_count_max) {
	pthread_mutex_init(&mq->lock, NULL);
	pthread_cond_init(&mq->cond_readable, NULL);
	pthread_cond_init(&mq->cond_writable, NULL);
	mq->data = malloc(msg_size*msg_count_max);
	mq->msg_size = msg_size;
	mq->msg_count_max = msg_count_max;
	mq->msg_count = 0;
	mq->head_idx = 0;
	mq->name = NULL;

	if(((msg_size*msg_count_max) > 0) && mq->data == NULL) return false;
	return true;
}

void pthread_mq_free(pthread_mq_t *mq) {
	free(mq->data);
}

void pthread_mq_cond(pthread_mq_t *mq) {
	if(pthread_mq_readable(mq)) pthread_cond_broadcast(&mq->cond_readable);
	if(pthread_mq_writable(mq)) pthread_cond_broadcast(&mq->cond_writable);
}

bool pthread_mq_reset(pthread_mq_t *mq) {
	if(pthread_mutex_lock(&mq->lock)) return false;
	mq->msg_count = 0;
	mq->head_idx = 0;
	pthread_mq_cond(mq);
	pthread_mutex_unlock(&mq->lock);
	return true;
}

bool pthread_mq_send_generic(pthread_mq_t *mq, void * data, bool to_front, bool block, const struct timespec *restrict abs_timeout) {
	int ret;

	//Lock queue
	if(pthread_mutex_timedlock(&mq->lock, abs_timeout)) return false;

	//Wait for queue to be in writable condition
	while(!pthread_mq_writable(mq)) {
		if(abs_timeout == NULL) {
			ret = pthread_cond_wait(&mq->cond_writable, &mq->lock);
		} else {
			ret = pthread_cond_timedwait(&mq->cond_writable, &mq->lock, abs_timeout);
		}
		if(ret) {
			pthread_mutex_unlock(&mq->lock);
			return false;
		}
	}
	printf("Writable\n");

	//Write data to queue
	size_t idx = ( ( mq->msg_count_max + mq->head_idx + (to_front?-1:mq->msg_count) ) % mq->msg_count_max );
	void *ptr = mq->data + (idx * mq->msg_size);
	mq->msg_count++;
	if(to_front) mq->head_idx = (mq->msg_count_max + mq->head_idx - 1) % mq->msg_count_max;
	memcpy(ptr, data, mq->msg_size);

	//Signal conditions and unlock
	pthread_mq_cond(mq);
	pthread_mutex_unlock(&mq->lock);
	return true;
}

int main() {
	pthread_mq_t myq;
	pthread_mq_init(&myq, 6, 5);
	pthread_mq_send_generic(&myq, "AHOJ1", false, true, NULL);
	pthread_mq_send_generic(&myq, "AHOJ2", false, true, NULL);
	pthread_mq_send_generic(&myq, "AHOJ3", true, true, NULL);
	pthread_mq_send_generic(&myq, "AHOJ4", true, true, NULL);
	pthread_mq_send_generic(&myq, "AHOJ5", false, true, NULL);
	//pthread_mq_send_generic(&myq, "AHOJ6", false, true, NULL);

	for(int i = 0; i<5; i++) {
		printf("%.6s\n", (char *)(myq.data+i*6));
	}
}
