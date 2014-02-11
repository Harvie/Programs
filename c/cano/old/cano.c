#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <linux/limits.h>


static const char *fd_to_path(int fd) {
	static char path[PATH_MAX];
	sprintf(path,"/proc/self/fd/%d",fd);
	int l = (int) readlink((char * __restrict__) path, path, PATH_MAX);
	path[l]=0;
	return path;
}

int at_close() {
	unsetenv("LD_PRELOAD");
	return system(getenv("AT_CLOSE"));
	/*
	static const char *command;
	if(!command) {
		command = getenv("AT_CLOSE");
		unsetenv("LD_PRELOAD");
	}
	fprintf(stderr,"%s\n",command);
	system(command);
	return 0;
	*/
}

int close(int fd) {
	//fprintf(stderr,"%s(%d,%s);\n",__func__,fd,fd_to_path(fd));
	static int (*close_orig)(int fd);
	if (!close_orig) {
		close_orig = dlsym(RTLD_NEXT, "close");
	}
	int ret = close_orig(fd);
	//int ret = syscall(SYS_close,fd);
	at_close();
	return ret;
}
int fclose(FILE *fd) {
	//fprintf(stderr,"%s(%d,%d,%s);\n",__func__,fileno(fd),fd,fd_to_path(fileno(fd)));
	static int (*fclose_orig)(FILE *fd);
	if (!fclose_orig) {
		fclose_orig = dlsym(RTLD_NEXT, "fclose");
	}
	int ret = fclose_orig(fd);
	at_close();
	return ret;
}
