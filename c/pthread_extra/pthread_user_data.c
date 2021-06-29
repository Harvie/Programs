#define __PTHREAD_EXTRA_INTERNAL

#include <assert.h>
#include <pthread.h>
#include <pthread_extra.h>

//Static array with user data for all thread handles
//TODO: perhaps use something more sophisticated like linked list?
pthread_user_data_internal_t pthread_user_data[PTHREAD_XTHREADS_MAX+1] = {{.tid=PTHREAD_XNULL}};

//Get pointer to internal record tied to specified thread
pthread_user_data_internal_t* pthread_user_data_internal(pthread_t thread) {
	//Return NULL if requested thread handle is NULL
	if(pthread_equal(thread, PTHREAD_XNULL)) return NULL;

	//Find if the thread is already registered, add it if not
	//FIXME: recycle slots of destroyed threads!!!
	pthread_t i;
	for(i = 0; i<PTHREAD_XTHREADS_MAX; i++) {
		if(pthread_equal(pthread_user_data[i].tid, PTHREAD_XNULL)) {
			//FIXME: Should be locking for addition to the array!!!!
			pthread_user_data[i+1].tid = PTHREAD_XNULL;
			pthread_user_data[i].tid = thread;
			break;
		}
		if(pthread_equal(pthread_user_data[i].tid, thread)) break;
	}
	//Return pointer
	return &pthread_user_data[i];
}

//Get pointer to user specified pointer of that thread
void** pthread_user_data_ptr(pthread_t thread) {
	return &pthread_user_data_internal(thread)->usr;
}

//Set user specified pointer for thread
void pthread_user_data_set(pthread_t thread, void *usr) {
	*(pthread_user_data_ptr(thread)) = usr;
}

//Get user specified pointer for thread
void* pthread_user_data_get(pthread_t thread) {
	return *(pthread_user_data_ptr(thread));
}

//Remove canceled thread from user data registry
int pthread_user_data_remove(pthread_t thread) {
	//FIXME: not implemented yet!
	(void) thread;
	return 0;
}

//User data cleanup handler to be registered with pthread_cleanup_push()
void pthread_user_data_cleanup( void * arg ) {
	pthread_t t = (pthread_t)arg;
	assert(pthread_equal(t, pthread_self()) && "Pthread_self() is not working in cleanup context!");
	pthread_user_data_remove(t);
}
