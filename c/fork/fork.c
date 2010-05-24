#include <stdio.h>
#include <pthread.h>

int main() {

	if(!pthread_create()) while(1) { printf("child\n"); sleep(1); }
	//return(0);
	while(1) { printf("parrent\n"); sleep(2); }

}
