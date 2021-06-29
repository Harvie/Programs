#define __PTHREAD_EXTRA_INTERNAL

#include <pthread.h>
#include <pthread_extra.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/resource.h>
//#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//#include <sys/time.h>

void pthread_pause_handler() {
	//Do nothing when there are more signals pending (to cleanup the queue)
	sigset_t pending;
	sigpending(&pending);
	if(sigismember(&pending, PTHREAD_XSIG_STOP)) return;

	//Keep waiting for signals until we are supposed to be running
	sigset_t sigset;
	sigfillset(&sigset);
	sigdelset(&sigset, PTHREAD_XSIG_STOP);
	if(!pthread_user_data_internal(pthread_self())->running) {
		sigsuspend(&sigset);
	}
}

void pthread_pause_enable() {
	//Add thread to internal registry
	pthread_user_data_internal(pthread_self());

	//Nesting signals too deep is not good for stack
	//You can get runtime stats using following command:
	//grep -i sig /proc/$(pgrep binary)/status
	struct rlimit sigq = {.rlim_cur = 32, .rlim_max=32};
	setrlimit(RLIMIT_SIGPENDING, &sigq);

	//Setup signal handler
	signal(PTHREAD_XSIG_STOP, pthread_pause_handler);

	//Unblock signal
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, PTHREAD_XSIG_STOP);
	pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);
}

void pthread_pause_disable() {
	//Add thread to internal registry
	pthread_user_data_internal(pthread_self());

	//Block signal
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, PTHREAD_XSIG_STOP);
	pthread_sigmask(SIG_BLOCK, &sigset, NULL);
}

int pthread_pause_reschedule(pthread_t thread) {
	//Send signal to initiate pause handler
	while(pthread_kill(thread, PTHREAD_XSIG_STOP) == EAGAIN) usleep(1000);
	return 0;
}

int pthread_pause(pthread_t thread) {
	//Set thread as paused and notify it via signal (wait when queue full)
	pthread_user_data_internal(thread)->running = 0;
	pthread_pause_reschedule(thread);
	return 0;
}

int pthread_unpause(pthread_t thread) {
	//Set thread as running and notify it via signal (wait when queue full)
	pthread_user_data_internal(thread)->running = 1;
	pthread_pause_reschedule(thread);
	return 0;
}



// Wrappers ///////////////////////////////////////////////////////////


typedef struct pthread_extra_wrapper_t {
	void *(*start_routine)(void *);
	void *restrict arg;
} pthread_extra_wrapper_t;

void *pthread_extra_thread_wrapper(void *arg) {
	pthread_extra_wrapper_t task = *((pthread_extra_wrapper_t*)arg);
	free(arg);

	//Register new thread to user data structure
	pthread_user_data_internal(pthread_self());

	//TODO: user_data should do this automaticaly?
	pthread_cleanup_push(pthread_user_data_cleanup, (void *)pthread_self());

	//Check if we should be running according to pthread_pause sub-scheduler
	pthread_pause_reschedule(pthread_self());

	return task.start_routine(task.arg);

	pthread_cleanup_pop(1); //Needed by pthread_cleanup_push() macro
}

int pthread_extra_create(pthread_t *restrict thread,
                          const pthread_attr_t *restrict attr,
                          void *(*start_routine)(void *),
                          void *restrict arg) {

	pthread_extra_wrapper_t *task = malloc(sizeof(pthread_extra_wrapper_t));
	assert(task != NULL);
	task->start_routine=start_routine;
	task->arg=arg;
	return pthread_create(thread, attr, pthread_extra_thread_wrapper, task);
}
