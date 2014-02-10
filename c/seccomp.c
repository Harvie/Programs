/* seccomp.c
 *
 * This demonstrates how to use SECCOMP_MODE_STRICT to sandbox code on Linux.
 */

#include <string.h>
#include <sys/prctl.h>
#include <linux/seccomp.h>
#include <sys/syscall.h>

#define DISPLAY(msg) (syscall( SYS_write, 2, msg, strlen(msg) ))

int main() {
	system("echo before");

	if(prctl(PR_SET_SECCOMP, SECCOMP_MODE_STRICT) == 0)
		DISPLAY("SECCOMP Enabled!\n"); else DISPLAY("SECCOMP Fail!\n");
	//fflush(NULL);

	system("echo after");
}
