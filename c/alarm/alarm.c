#include <stdio.h>
#include <signal.h>

static void alrm_handler(int signo) {
	alarm(1);
	printf("LOL!\n");
}

int main(int argc, char *argv[]) {

	//signal(SIGALRM, &alrm_handler);
	//alarm(1);
	
	while(1) {
	  sleep(1);
		printf(".");
	}

}
