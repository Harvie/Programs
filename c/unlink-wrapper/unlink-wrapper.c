#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
//#include <unistd.h>
//#define RTLD_NEXT ((void *) -1l)


int unlinkat(int dirfd, const char *pathname, int flags)
{
	puts("unlinkat");
	int (*orig_unlinkat)(int, const char*, int) = dlsym(RTLD_NEXT, "unlinkat");
	return orig_unlinkat(dirfd,pathname,flags);
}

//int unlink(const char *pathname)
//ssize_t write(int fd, const void *buf, size_t count)
