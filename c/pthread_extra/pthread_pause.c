#define __PTHREAD_EXTRA_INTERNAL

#include <pthread.h>
#include <pthread_extra.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/resource.h>
//#include <stdio.h>
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
	//Block signal
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, PTHREAD_XSIG_STOP);
	pthread_sigmask(SIG_BLOCK, &sigset, NULL);
}

int pthread_pause(pthread_t thread) {
	//Set thread as paused and notify it via signal (wait when queue full)
	pthread_user_data_internal(thread)->running = 0;
	while(pthread_kill(thread, PTHREAD_XSIG_STOP) == EAGAIN) usleep(1000);
	return 0;
}

int pthread_unpause(pthread_t thread) {
	//Set thread as running and notify it via signal (wait when queue full)
	pthread_user_data_internal(thread)->running = 1;
	while(pthread_kill(thread, PTHREAD_XSIG_STOP) == EAGAIN) usleep(1000);
	return 0;
}
