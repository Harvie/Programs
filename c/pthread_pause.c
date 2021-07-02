//Filename: pthread_pause.c
//Author: Tomas 'Harvie' Mudrunka 2021
//Build: CFLAGS=-lpthread make pthread_pause; ./pthread_pause
//Test: valgrind --tool=helgrind ./pthread_pause

//I've wrote this code as excercise to solve following stack overflow question:
// https://stackoverflow.com/questions/9397068/how-to-pause-a-pthread-any-time-i-want/68119116#68119116

#define _GNU_SOURCE //pthread_yield() needs this
#include <signal.h>
#include <pthread.h>
//#include <pthread_extra.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>
#include <time.h>

#define PTHREAD_XSIG_STOP (SIGRTMIN+0)
#define PTHREAD_XSIG_CONT (SIGRTMIN+1)
#define PTHREAD_XSIGRTMIN (SIGRTMIN+2) //First unused RT signal

pthread_t main_thread;
sem_t pthread_pause_sem;
pthread_once_t pthread_pause_once_ctrl = PTHREAD_ONCE_INIT;

void pthread_pause_once(void) {
	sem_init(&pthread_pause_sem, 0, 1);
}

#define pthread_pause_init() (pthread_once(&pthread_pause_once_ctrl, &pthread_pause_once))

#define NSEC_PER_SEC (1000*1000*1000)
// timespec_normalise() from https://github.com/solemnwarning/timespec/
struct timespec timespec_normalise(struct timespec ts)
{
	while(ts.tv_nsec >= NSEC_PER_SEC) {
		++(ts.tv_sec); ts.tv_nsec -= NSEC_PER_SEC;
	}
	while(ts.tv_nsec <= -NSEC_PER_SEC) {
		--(ts.tv_sec); ts.tv_nsec += NSEC_PER_SEC;
	}
	if(ts.tv_nsec < 0) { // Negative nanoseconds isn't valid according to POSIX.
		--(ts.tv_sec); ts.tv_nsec = (NSEC_PER_SEC + ts.tv_nsec);
	}
	return ts;
}

void pthread_nanosleep(struct timespec t) {
	//Sleep calls on Linux get interrupted by signals, causing premature wake
	//Pthread (un)pause is built using signals
	//Therefore we need self-restarting sleep implementation
	//IO timeouts are restarted by SA_RESTART, but sleeps do need explicit restart
	//We also need to sleep using absolute time, because relative time is paused
	//You should use this in any thread that gets (un)paused

	struct timespec wake;
	clock_gettime(CLOCK_MONOTONIC, &wake);

	t = timespec_normalise(t);
	wake.tv_sec += t.tv_sec;
	wake.tv_nsec += t.tv_nsec;
	wake = timespec_normalise(wake);

	while(clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &wake, NULL)) if(errno!=EINTR) break;
	return;
}
void pthread_nsleep(time_t s, long ns) {
	struct timespec t;
	t.tv_sec = s;
	t.tv_nsec = ns;
	pthread_nanosleep(t);
}

void pthread_sleep(time_t s) {
	pthread_nsleep(s, 0);
}

void pthread_pause_yield() {
	//Call this to give other threads chance to run
	//Wait until last (un)pause action gets finished
	sem_wait(&pthread_pause_sem);
	sem_post(&pthread_pause_sem);
        //usleep(0);
	//nanosleep(&((const struct timespec){.tv_sec=0,.tv_nsec=1}), NULL);
	//pthread_nsleep(0,1); //pthread_yield() is not enough, so we use sleep
	pthread_yield();
}

void pthread_pause_handler(int signal) {
    //Do nothing when there are more signals pending (to cleanup the queue)
    //This is no longer needed, since we use semaphore to limit pending signals
    /*
    sigset_t pending;
    sigpending(&pending);
    if(sigismember(&pending, PTHREAD_XSIG_STOP)) return;
    if(sigismember(&pending, PTHREAD_XSIG_CONT)) return;
    */

    //Post semaphore to confirm that signal is handled
    sem_post(&pthread_pause_sem);
    //Suspend if needed
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
    //This is no longer needed, since we use semaphores
    //struct rlimit sigq = {.rlim_cur = 32, .rlim_max=32};
    //setrlimit(RLIMIT_SIGPENDING, &sigq);

    pthread_pause_init();

    //Prepare sigset
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, PTHREAD_XSIG_STOP);
    sigaddset(&sigset, PTHREAD_XSIG_CONT);

    //Register signal handlers
    //signal(PTHREAD_XSIG_STOP, pthread_pause_handler);
    //signal(PTHREAD_XSIG_CONT, pthread_pause_handler);
    //We now use sigaction() instead of signal(), because it supports SA_RESTART
    const struct sigaction pause_sa = {
		.sa_handler = pthread_pause_handler,
		.sa_mask = sigset,
		.sa_flags = SA_RESTART,
		.sa_restorer = NULL
    };
    sigaction(PTHREAD_XSIG_STOP, &pause_sa, NULL);
    sigaction(PTHREAD_XSIG_CONT, &pause_sa, NULL);

    //UnBlock signals
    pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);
}

void pthread_pause_disable() {
	//This is important for when you want to do some signal unsafe stuff
	//Eg.: locking mutex, calling printf() which has internal mutex, etc...
	//After unlocking mutex, you can enable pause again.

	pthread_pause_init();

	//Make sure all signals are dispatched before we block them
	sem_wait(&pthread_pause_sem);

	//Block signals
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, PTHREAD_XSIG_STOP);
	sigaddset(&sigset, PTHREAD_XSIG_CONT);
	pthread_sigmask(SIG_BLOCK, &sigset, NULL);

	sem_post(&pthread_pause_sem);
}


int pthread_pause(pthread_t thread) {
	sem_wait(&pthread_pause_sem);
	//If signal queue is full, we keep retrying
	while(pthread_kill(thread, PTHREAD_XSIG_STOP) == EAGAIN) usleep(1000);
	pthread_pause_yield();
	return 0;
}

int pthread_unpause(pthread_t thread) {
	sem_wait(&pthread_pause_sem);
	//If signal queue is full, we keep retrying
	while(pthread_kill(thread, PTHREAD_XSIG_CONT) == EAGAIN) usleep(1000);
	pthread_pause_yield();
	return 0;
}

void *thread_test() {
    //Whole process dies if you kill thread immediately before it is pausable
    //pthread_pause_enable();
    while(1) {
	//Printf() is not async signal safe (because it holds internal mutex),
	//you should call it only with pause disabled!
	//Will throw helgrind warnings anyway, not sure why...
	//See: man 7 signal-safety
	pthread_pause_disable();
        printf("Running!\n");
	pthread_pause_enable();

	//Pausing main thread should not cause deadlock
	//We pause main thread here just to test it is OK
	pthread_pause(main_thread);
	//pthread_nsleep(0, 1000*1000);
	pthread_unpause(main_thread);

	//Wait for a while
	//pthread_nsleep(0, 1000*1000*100);
	pthread_unpause(main_thread);
    }
}

int main() {
    pthread_t t;
    main_thread = pthread_self();
    pthread_pause_enable(); //Will get inherited by all threads from now on
    //you need to call pthread_pause_enable (or disable) before creating threads,
    //otherwise first (un)pause signal will kill whole process
    pthread_create(&t, NULL, thread_test, NULL);

    while(1) {
        pthread_pause(t);
        printf("PAUSED\n");
        pthread_sleep(3);

        printf("UNPAUSED\n");
        pthread_unpause(t);
        pthread_sleep(1);

	/*
	pthread_pause_disable();
        printf("RUNNING!\n");
	pthread_pause_enable();
	*/
	pthread_pause(t);
	pthread_unpause(t);
    }

    pthread_join(t, NULL);
    printf("DIEDED!\n");
}
