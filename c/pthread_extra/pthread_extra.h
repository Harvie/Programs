#ifndef __PTHREAD_EXTRA_H__
#define __PTHREAD_EXTRA_H__

#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#define PTHREAD_XTIME_NOBLOCK (&(struct timespec){ .tv_sec = 0, .tv_nsec = 0 })
#define PTHREAD_XTIME_FOREVER NULL

// Message queues

#define PTHREAD_XMQ_FRONT true
#define PTHREAD_XMQ_BACK false

#define PTHREAD_XMQ_RECV false
#define PTHREAD_XMQ_PEEK true

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

bool pthread_mq_init(pthread_mq_t *mq, size_t msg_size, size_t msg_count_max);
void pthread_mq_free(pthread_mq_t *mq);
size_t pthread_mq_waiting(pthread_mq_t *mq);
bool pthread_mq_reset(pthread_mq_t *mq);
bool pthread_mq_send_generic(pthread_mq_t *mq, void * data, bool to_front, const struct timespec *restrict abs_timeout);
bool pthread_mq_receive_generic(pthread_mq_t *mq, void * data, bool peek, const struct timespec *restrict abs_timeout);

// Multi mutex locking

#define pthread_mutex_swap(a, b) ({ pthread_mutex_t *s; s = (a); a = (b); b = s; })

#define pthread_mutex_lock_two(a,b) pthread_mutex_timedlock_multi_generic((pthread_mutex_t *[2]){(a), (b)}, 2, true, NULL)
#define pthread_mutex_timedlock_two(a,b,tm) pthread_mutex_timedlock_multi_generic((pthread_mutex_t *[2]){(a), (b)}, 2, true, (tm))
#define pthread_mutex_trylock_two(a,b) pthread_mutex_timedlock_multi_generic((pthread_mutex_t *[2]){(a), (b)}, 2, false, NULL)

#define pthread_mutex_lock_multi(lcks,cnt) pthread_mutex_timedlock_multi_generic((lcks),(cnt),true,NULL)
#define pthread_mutex_timedlock_multi(lcks,cnt,tm) pthread_mutex_timedlock_multi_generic((lcks),(cnt),true,(tm))
#define pthread_mutex_trylock_multi(lcks,cnt) pthread_mutex_timedlock_multi_generic((lcks),(cnt),false,NULL)

int pthread_mutex_timedlock_multi_generic(pthread_mutex_t **lck, int cnt, bool block, const struct timespec *restrict abs_timeout);


#endif //__PTHREAD_EXTRA_H__
