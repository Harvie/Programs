//Filename: pause.c
//Author: Tomas 'Harvie' Mudrunka 2021
//Build: CFLAGS=-lpthread make pause; ./pause
//Test: valgrind --tool=helgrind ./pause

//I've wrote this code as excercise to solve following stack overflow question:
// https://stackoverflow.com/questions/9397068/how-to-pause-a-pthread-any-time-i-want/68119116#68119116

#include <signal.h>
#include <pthread.h>
//#include <pthread_extra.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>

#define PTHREAD_XSIG_STOP (SIGRTMIN+0)
#define PTHREAD_XSIG_CONT (SIGRTMIN+1)
#define PTHREAD_XSIGRTMIN (SIGRTMIN+2) //First unused RT signal

void pthread_pause_handler(int signal) {
    //Do nothing when there are more signals pending (to cleanup the queue)
    sigset_t pending;
    sigpending(&pending);
    if(sigismember(&pending, PTHREAD_XSIG_STOP)) return;
    if(sigismember(&pending, PTHREAD_XSIG_CONT)) return;

    if(signal == PTHREAD_XSIG_STOP) {
    	sigset_t sigset;
    	sigfillset(&sigset);
    	sigdelset(&sigset, PTHREAD_XSIG_STOP);
    	sigdelset(&sigset, PTHREAD_XSIG_CONT);
    	sigsuspend(&sigset); //Wait for next signal
    } else return;
}

void pthread_pause_enable() {
    //Having signal queue too deep might not be necessary
    //It can be limited using RLIMIT_SIGPENDING
    //You can get runtime SigQ stats using following command:
    //grep -i sig /proc/$(pgrep binary)/status
    struct rlimit sigq = {.rlim_cur = 32, .rlim_max=32};
    setrlimit(RLIMIT_SIGPENDING, &sigq);

    //Register signal handlers
    signal(PTHREAD_XSIG_STOP, pthread_pause_handler);
    signal(PTHREAD_XSIG_CONT, pthread_pause_handler);

    //UnBlock signals
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, PTHREAD_XSIG_STOP);
    sigaddset(&sigset, PTHREAD_XSIG_CONT);
    pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);
}

void pthread_pause_disable() {
	//This is important for when you want to do some signal unsafe stuff
	//Eg.: locking mutex, calling printf() which has internal mutex, etc...
	//After unlocking mutex, you can enable pause again.

	//Block signals
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, PTHREAD_XSIG_STOP);
	sigaddset(&sigset, PTHREAD_XSIG_CONT);
	pthread_sigmask(SIG_BLOCK, &sigset, NULL);
}


int pthread_pause(pthread_t thread) {
	//If signal queue is full, we keep retrying
	while(pthread_kill(thread, PTHREAD_XSIG_STOP) == EAGAIN) usleep(1000);
	return 0;
}

int pthread_unpause(pthread_t thread) {
	//If signal queue is full, we keep retrying
	while(pthread_kill(thread, PTHREAD_XSIG_CONT) == EAGAIN) usleep(1000);
	return 0;
}

void *thread_test() {
    //Whole process dies if you kill thread immediately before it is pausable
    //pthread_pause_enable();
    while(1) {
        usleep(1000*300);
	//Printf() is not async signal safe (because it holds internal mutex),
	//you should call it only with pause disabled!
	//Will throw helgrind warnings anyway, not sure why...
	//See: man 7 signal-safety
	pthread_pause_disable();
        printf("Running!\n");
	pthread_pause_enable();
    }
}

int main() {
    pthread_t t;
    pthread_pause_enable(); //Will get inherited by all threads from now on
    //you need to call pthread_pause_enable (or disable) before creating threads,
    //otherwise first signal will kill whole process
    pthread_create(&t, NULL, thread_test, NULL);

    while(1) {
        pthread_pause(t);
        printf("PAUSED\n");
        sleep(3);

        printf("UNPAUSED\n");
        pthread_unpause(t);
        sleep(1);
    }

    pthread_join(t, NULL);
    printf("DIEDED!\n");
}
