/*
 * CFLAGS=-lpthread make pthread_msgqueue
 */

#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct pthread_mq_t {
	static pthread_mutex_t lock;
	static pthread_cond_t cond_readable;
	static pthread_cond_t cond_writable;
	void * data;
	size_t msg_size;
	size_t msg_count;
	size_t msg_count_max;
	size_t head_idx;
	char * name;
} pthread_mq_t;

bool pthread_mq_readable(pthread_mq_t *mq) { return (mq->count > 0); }
bool pthread_mq_writable(pthread_mq_t *mq) { return (mq->count < mq->count_max); }

bool pthread_mq_init(pthread_mq_t *mq, size_t msg_size, size_t msg_count_max) {
	mq->lock = PTHREAD_MUTEX_INITIALIZER;
	mq->cond_readable = PTHREAD_COND_INITIALIZER;
	mq->cond_writable = PTHREAD_COND_INITIALIZER;
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

bool pthread_mq_send_generic(pthread_mq_t *mq, void * data, bool to_front, bool block, const struct timespec *restrict abs_timeout) {
	pthread_mutex_timedlock(&mq->lock, abs_timeout);

	while(!pthread_mq_writable(mq)) {
		pthread_mutex_unlock(&mq->lock);
		return false;
	}

	size_t idx = ( ( mq->count_max + mq->head_idx + (to_front?-1:mq->count) ) % mq->count_max );
	void *ptr = mq->data + (idx * mq->msg_size);
	mq->msg_count++;
	memcpy(ptr, data, mq->msg_size);

	pthread_mq_cond(mq);
	pthread_mutex_unlock(&mq->lock);
	return true;
}

/*
              mq_timedreceive(3)   mq_timedreceive(2)

              mq_timedsend(3)      mq_timedsend(2)

void vQueueDelete( QueueHandle_t xQueue );
BaseType_t xQueueReset( QueueHandle_t xQueue );

 BaseType_t xQueueSend(
                            QueueHandle_t xQueue,
                            const void * pvItemToQueue,
                            TickType_t xTicksToWait
                         );
 BaseType_t xQueueReceive(
                               QueueHandle_t xQueue,
                               void *pvBuffer,
                               TickType_t xTicksToWait
                            );

*/

int main() {

}
