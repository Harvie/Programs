/*
 * seccomp.c (Harvie 2o14)
 *
 * This demonstrates how to use SECCOMP_MODE_STRICT to sandbox code on Linux.
 * You need kernel compiled with CONFIG_SECCOMP=y.
 * This prohibits everything except read(2), write(2), _exit(2), and sigreturn(2).
 * Trying to use other syscalls will result in SIGKILL.
 * If you need to enable more syscalls you can use SECCOMP_MODE_FILTER instead.
 * See man 2 prctl for more...
 *
 */

#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <linux/seccomp.h>
#include <sys/syscall.h>

#define DISPLAY(msg) (syscall( SYS_write, 2, msg, strlen(msg) ))
#define exit(status) { syscall( SYS_exit, status ); abort(); }

int main() {
	system("echo before");

	if(prctl(PR_SET_SECCOMP, SECCOMP_MODE_STRICT) == 0)
		DISPLAY("SECCOMP Enabled!\n"); else DISPLAY("SECCOMP Fail!\n");
	//fflush(NULL);

	system("echo after");
	exit(0);
}
