#define __PTHREAD_EXTRA_INTERNAL

#include <pthread.h>
#include <pthread_extra.h>
#include <signal.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>
#include <sys/resource.h>
//#include <sys/siginfo.h>
//#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//#include <sys/time.h>

//Mutex that ensures proper serialization of (un)pause calls
//pthread_mutex_t pthread_pause_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

//Semaphore that ensures proper serialization of (un)pause signals
sem_t pthread_pause_sem;
//Once control to init the semaphore (and possibly other stuff)
pthread_once_t pthread_pause_once_ctrl = PTHREAD_ONCE_INIT;
void pthread_pause_once(void) {
	sem_init(&pthread_pause_sem, 0, 1);
}
void pthread_pause_init() { pthread_once(&pthread_pause_once_ctrl, &pthread_pause_once); }

///When this variable is nonzero, only referenced thread is allowed to run
///Access has to be protected by pthread_user_data_lock() and pthread_pause_sem;
pthread_t pthread_pause_holder = PTHREAD_XNULL;

void pthread_pause_handler(const int signal, siginfo_t *info, void *ptr) {
	(void)signal; (void)info; (void)ptr;
	int run = info->si_value.sival_int;
	//(void)td;

	//Post semaphore to confirm that signal is handled
	sem_post(&pthread_pause_sem);

	//Keep waiting for signals until we are supposed to be running
	if(!run) {
		sigset_t sigset;
		sigfillset(&sigset);
		sigdelset(&sigset, PTHREAD_XSIG_STOP);
		sigsuspend(&sigset);
	}
}

void pthread_pause_enable() {
	//Add thread to internal registry
	//pthread_user_data_internal(pthread_self());

	//Nesting signals too deep is not good for stack
	//You can get runtime stats using following command:
	//grep -i sig /proc/$(pgrep binary)/status
	//struct rlimit sigq = {.rlim_cur = 32, .rlim_max=32};
	//setrlimit(RLIMIT_SIGPENDING, &sigq);

	pthread_pause_init(); //Make sure semaphore is init'd

	//Prepare signal mask
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, PTHREAD_XSIG_STOP);

	//Setup signal handler
	//signal(PTHREAD_XSIG_STOP, pthread_pause_handler);
	const struct sigaction pause_sa = {
		.sa_sigaction = pthread_pause_handler,
		.sa_mask = sigset,
		.sa_flags = SA_SIGINFO | SA_RESTART,
		.sa_restorer = NULL
	};
	sigaction(PTHREAD_XSIG_STOP, &pause_sa, NULL);

	//Unblock signal
	pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);
}

void pthread_pause_disable() {
	//Add thread to internal registry
	//pthread_user_data_internal(pthread_self());

	pthread_pause_init(); //Make sure semaphore is init'd

	//Block signal
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, PTHREAD_XSIG_STOP);

	//Make sure all signals are dispatched before we block them
	sem_wait(&pthread_pause_sem);
	pthread_sigmask(SIG_BLOCK, &sigset, NULL);
	sem_post(&pthread_pause_sem);
}

/*
int pthread_pause_reschedule(pthread_t thread) {
	//Send signal to initiate pause handler
	//printf("SND: %p\n", (void *)pthread_user_data_internal(thread));
	//while(pthread_kill(thread, PTHREAD_XSIG_STOP) == EAGAIN) usleep(1000);
	while(pthread_sigqueue(thread, PTHREAD_XSIG_STOP,
		(const union sigval){.sival_ptr=pthread_user_data_internal(thread)}
		) == EAGAIN) usleep(1000);
	return 0;
}
*/

int pthread_pause_reschedule(pthread_t thread) {
	//Decide if the thread should run and signal it

	//Wait for semaphore which means signal queue is empty
	pthread_pause_init(); //Make sure semaphore is init'd
	sem_wait(&pthread_pause_sem);

	//Only call this if you already acquired pthread_pause_sem semaphore!!!!
	//Otherwise call pthread_pause_reschedule()

	pthread_user_data_lock();
	//Check if thread has running flag
	int run = (pthread_user_data_internal(thread)->running);
	//Check if privileged (single thread) mode is active
	if((pthread_pause_holder != PTHREAD_XNULL) && !pthread_equal(pthread_pause_holder, thread)) {
		run = 0;
	}
	pthread_user_data_unlock();

	//Send signal to initiate pause handler (keep trying while SigQueue is full)
	//while(pthread_kill(thread, PTHREAD_XSIG_STOP) == EAGAIN) usleep(1000);
	while(pthread_sigqueue(thread, PTHREAD_XSIG_STOP,
		(const union sigval){.sival_int=run}
		) == EAGAIN) usleep(1000);

	//Wait for signal to be delivered
	sem_wait(&pthread_pause_sem);
	sem_post(&pthread_pause_sem);

	return 0;
}

int pthread_extra_yield() {
	//Yield to both schedulers
	pthread_pause_reschedule(pthread_self());
	return pthread_yield();
}

int pthread_pause(pthread_t thread) {
	//Set thread as paused and notify it via signal (wait when queue full)
	pthread_user_data_lock();
	pthread_user_data_internal(thread)->running = 0;
	pthread_pause_reschedule(thread);
	pthread_user_data_unlock();
	return 0;
}

int pthread_unpause(pthread_t thread) {
	//Set thread as running and notify it via signal (wait when queue full)
	pthread_user_data_lock();
	pthread_user_data_internal(thread)->running = 1;
	pthread_pause_reschedule(thread);
	pthread_user_data_unlock();
	return 0;
}

int pthread_pause_all() {
	pthread_user_data_lock();
	if(pthread_pause_holder!=PTHREAD_XNULL) assert(pthread_equal(pthread_pause_holder, pthread_self()));
	pthread_pause_holder = pthread_self();
	pthread_user_data_internal_iterate(&pthread_pause_reschedule, NULL);
	pthread_user_data_unlock();
	return 0;
}

int pthread_unpause_all() {
	pthread_user_data_lock();
	if(pthread_pause_holder!=PTHREAD_XNULL) assert(pthread_equal(pthread_pause_holder, pthread_self()));
	pthread_pause_holder = PTHREAD_XNULL;
	pthread_user_data_internal_iterate(&pthread_pause_reschedule, NULL);
	pthread_user_data_unlock();
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
	//pthread_pause_reschedule(pthread_self());
	pthread_extra_yield();

	//Run actual task
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
