/*
 * CFLAGS=-lpthread make pthread_multi
 */

#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#define pthread_mutex_swap(a, b) ({ pthread_mutex_t *s; s = (a); a = (b); b = s; })

#define pthread_mutex_lock_two(a,b) pthread_mutex_timedlock_multi_generic((pthread_mutex_t *[2]){(a), (b)}, 2, true, NULL)
#define pthread_mutex_timedlock_two(a,b,tm) pthread_mutex_timedlock_multi_generic((pthread_mutex_t *[2]){(a), (b)}, 2, true, (tm))
#define pthread_mutex_trylock_two(a,b) pthread_mutex_timedlock_multi_generic((pthread_mutex_t *[2]){(a), (b)}, 2, false, NULL)

#define pthread_mutex_lock_multi(lcks,cnt) pthread_mutex_timedlock_multi_generic((lcks),(cnt),true,NULL)
#define pthread_mutex_timedlock_multi(lcks,cnt,tm) pthread_mutex_timedlock_multi_generic((lcks),(cnt),true,(tm))
#define pthread_mutex_trylock_multi(lcks,cnt) pthread_mutex_timedlock_multi_generic((lcks),(cnt),false,NULL)

/*
//This is first prototype for two mutexes only, it is useful in order to understand how this all works
//Currently it was replaced by macro with the same name
int pthread_mutex_timedlock_two(pthread_mutex_t *a, pthread_mutex_t *b, const struct timespec *restrict abs_timeout) {
	int ret;

	while(1) {
		//Wait for the first mutex
		ret = (abs_timeout==NULL ? pthread_mutex_lock(a) : pthread_mutex_timedlock(a, abs_timeout));
		if(ret) return ret; //Something is fishy about this mutex. Abort!

		//Try to lock the second mutex
		ret = pthread_mutex_trylock(b);
		if(!ret) return ret; //Locked BOTH Hooray!

		//Unlock first if second failed to prevent deadlocks
		pthread_mutex_unlock(a);

		//Swap mutexes, so we will try to block on the second mutex next time:
		pthread_mutex_swap(a, b);
		//printf("Retry!\n");
	}
}
*/

int pthread_mutex_timedlock_multi_generic(pthread_mutex_t **lck, int cnt, bool block, const struct timespec *restrict abs_timeout) {
	int ret, locked;

	while(1) {
		//Try to lock all mutexes
		for(int i = 0; i<cnt; i++) {
			//Block only on the first one
			if(block && i==0) {
				ret = (abs_timeout==NULL ? pthread_mutex_lock(lck[i]) : pthread_mutex_timedlock(lck[i], abs_timeout));
				if(ret) return ret; //Something went wrong
				continue;
			}
			//Then try if we can lock the rest
			ret = pthread_mutex_trylock(lck[i]);
			if(ret) {
				//Cannot lock this one
				locked = i;
				printf("Cannot lock #%d!\n", i);
				break;
			}
			locked = i;
		}

		//Check if we managed to lock all locks
		if(locked == cnt) return 0;

		//Unlock all mutexes that we managed to lock so far
		for(int i = 0;i<locked;i++) {
			printf("Unlocking #%d\n", i);
			pthread_mutex_unlock(lck[i]);
		}
		if(!block) return ret; //We do not want to block, just trying

		//Try to block on locked mutex in next round
		pthread_mutex_swap(lck[0],lck[locked]);
	}
}

int main() {
	//Prepare mutex array for tests
	static pthread_mutex_t la = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t lb = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t lc = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t ld = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t *lck[4] = {&la, &lb, &lc, &ld};

	//Set timeout
	struct  timespec tm;
	clock_gettime(CLOCK_REALTIME, &tm);
	tm.tv_sec  += 5;

	//Lock one of the locks for testing
	pthread_mutex_lock(lck[2]);

	if(!pthread_mutex_timedlock_multi(lck, 4, &tm)) {
	//if(!pthread_mutex_timedlock_two(&la, lck[2], &tm)) {
		printf("LOCKED!\n");
	} else {
		printf("FAILED!\n");
	}

}
