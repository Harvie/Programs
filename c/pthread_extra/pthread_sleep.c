#include <pthread_extra.h>
#include <time.h>
#include <errno.h>

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
