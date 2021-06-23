#include <pthread.h>
#include <pthread_extra.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

bool pthread_mq_readable(pthread_mq_t *mq) { return (mq->msg_count > 0); }
bool pthread_mq_writable(pthread_mq_t *mq) { return (mq->msg_count < mq->msg_count_max); }

bool pthread_mq_init(pthread_mq_t *mq, size_t msg_size, size_t msg_count_max) {
	pthread_mutex_init(&mq->lock, NULL);
	pthread_cond_init(&mq->cond_readable, NULL);
	pthread_cond_init(&mq->cond_writable, NULL);
	//pthread_mutexattr_setclock(&mq->lock, CLOCK_MONOTONIC);
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

size_t pthread_mq_waiting(pthread_mq_t *mq) {
	return mq->msg_count;
}

bool pthread_mq_reset(pthread_mq_t *mq) {
	if(pthread_mutex_lock(&mq->lock)) return false;
	mq->msg_count = 0;
	mq->head_idx = 0;
	pthread_mq_cond(mq);
	pthread_mutex_unlock(&mq->lock);
	return true;
}

bool pthread_mq_send_generic(pthread_mq_t *mq, void * data, bool to_front, const struct timespec *restrict abs_timeout) {
	//printf("S-Timed: %p\n", abs_timeout);
	int ret;

	//Lock queue
	if(pthread_mutex_timedlock(&mq->lock, abs_timeout)) return false;

	//Wait for queue to be in writable condition
	while(!pthread_mq_writable(mq)) {
		//printf("S+Timed: %p\n", abs_timeout);
		if(abs_timeout == NULL) {
			ret = pthread_cond_wait(&mq->cond_writable, &mq->lock);
		} else {
			//printf("STimed: %p\n", abs_timeout);
			//assert(abs_timeout != NULL);
			ret = pthread_cond_timedwait(&mq->cond_writable, &mq->lock, abs_timeout);
		}
		if(ret) {
			pthread_mutex_unlock(&mq->lock);
			return false;
		}
	}

	//Write data to queue
	size_t idx = ( ( mq->head_idx + (to_front?mq->msg_count_max-1:mq->msg_count) ) % mq->msg_count_max );
	void *ptr = mq->data + (idx * mq->msg_size);
	mq->msg_count++;
	if(to_front) mq->head_idx = (mq->msg_count_max + mq->head_idx - 1) % mq->msg_count_max;
	memcpy(ptr, data, mq->msg_size);

	//Signal conditions and unlock
	pthread_mq_cond(mq);
	pthread_mutex_unlock(&mq->lock);
	return true;
}

bool pthread_mq_receive_generic(pthread_mq_t *mq, void * data, bool peek, const struct timespec *restrict abs_timeout) {
	int ret;

	//Lock queue
	if(pthread_mutex_timedlock(&mq->lock, abs_timeout)) return false;

	//Wait for queue to be in readable condition
	while(!pthread_mq_readable(mq)) {
		if(abs_timeout == NULL) {
			ret = pthread_cond_wait(&mq->cond_readable, &mq->lock);
		} else {
			//printf("RTimed: %p\n", abs_timeout);
			//assert(abs_timeout != NULL);
			ret = pthread_cond_timedwait(&mq->cond_readable, &mq->lock, abs_timeout);
		}
		if(ret) {
			pthread_mutex_unlock(&mq->lock);
			return false;
		}
	}

	//Read data from queue
	void *ptr = mq->data + (mq->head_idx * mq->msg_size);
	memcpy(data, ptr, mq->msg_size);

	//Delete data from queue if not peeking
	if(!peek) {
		mq->msg_count--;
		mq->head_idx = (mq->head_idx+1) % mq->msg_count_max;
	}

	//Signal conditions and unlock
	pthread_mq_cond(mq);
	pthread_mutex_unlock(&mq->lock);
	return true;
}
