#ifndef __PTHREAD_EXTRA_H__
#define __PTHREAD_EXTRA_H__

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>
//#include <time.h>

#define PTHREAD_XTIME_NOBLOCK (&(struct timespec){ .tv_sec = 0, .tv_nsec = 0 })
#define PTHREAD_XTIME_FOREVER NULL

// User data

#define PTHREAD_XTHREADS_MAX 65535
#define PTHREAD_XNULL ((pthread_t)NULL)

#ifdef __PTHREAD_EXTRA_INTERNAL
typedef struct pthread_user_data_internal_t {
	pthread_t tid; //Thread ID
	sig_atomic_t running; //Internaly used by pthread_pause
	void *usr; //User pointer
} pthread_user_data_internal_t;

pthread_user_data_internal_t* pthread_user_data_internal(pthread_t thread);
#endif //__PTHREAD_EXTRA_INTERNAL

void** pthread_user_data_ptr(pthread_t thread);
void*  pthread_user_data_get(pthread_t thread);
void   pthread_user_data_set(pthread_t thread, void *usr);

// Pausing

//GDB: handle SIG34 nostop noprint
#define PTHREAD_XSIG_STOP (SIGRTMIN+0)
#define PTHREAD_XSIG_CONT (SIGRTMIN+1)
#define PTHREAD_XSIGRTMIN (SIGRTMIN+2) //First unused RT signal

void pthread_unpause_handler();
void pthread_pause_handler();
void pthread_pause_enable();
int pthread_pause(pthread_t thread);
int pthread_unpause(pthread_t thread);

// Message queues

#define PTHREAD_XMQ_FRONT true
#define PTHREAD_XMQ_BACK false

#define PTHREAD_XMQ_RECV false
#define PTHREAD_XMQ_PEEK true

typedef struct pthread_mq_t {
	pthread_mutex_t lock;
	pthread_cond_t cond_readable;
	pthread_cond_t cond_writable;
	uint8_t * data;
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

#define pthread_mutex_swap(a, b) { pthread_mutex_t *s; s = (a); a = (b); b = s; }

#define pthread_mutex_lock_two(a,b) pthread_mutex_timedlock_multi_generic((pthread_mutex_t *[2]){(a), (b)}, 2, true, NULL)
#define pthread_mutex_timedlock_two(a,b,tm) pthread_mutex_timedlock_multi_generic((pthread_mutex_t *[2]){(a), (b)}, 2, true, (tm))
#define pthread_mutex_trylock_two(a,b) pthread_mutex_timedlock_multi_generic((pthread_mutex_t *[2]){(a), (b)}, 2, false, NULL)

#define pthread_mutex_lock_multi(lcks,cnt) pthread_mutex_timedlock_multi_generic((lcks),(cnt),true,NULL)
#define pthread_mutex_timedlock_multi(lcks,cnt,tm) pthread_mutex_timedlock_multi_generic((lcks),(cnt),true,(tm))
#define pthread_mutex_trylock_multi(lcks,cnt) pthread_mutex_timedlock_multi_generic((lcks),(cnt),false,NULL)

int pthread_mutex_timedlock_multi_generic(pthread_mutex_t **lck, int cnt, bool block, const struct timespec *restrict abs_timeout);


#endif //__PTHREAD_EXTRA_H__
