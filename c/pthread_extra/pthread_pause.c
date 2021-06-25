#include <pthread.h>
#include <pthread_extra.h>
#include <signal.h>

void pthread_unpause_handler() {
	//NOP
}

void pthread_pause_handler() {
	sigset_t sigset;
	sigfillset(&sigset);
	sigdelset(&sigset, PTHREAD_XSIG_CONT);
	sigsuspend(&sigset);
}

void pthread_pause_enable() {
	signal(PTHREAD_XSIG_STOP, pthread_pause_handler);
	signal(PTHREAD_XSIG_CONT, pthread_unpause_handler);
}
