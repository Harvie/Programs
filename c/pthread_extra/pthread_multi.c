#include <pthread_extra.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

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
