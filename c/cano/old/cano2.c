#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#define i { srand(23); printf("%s();\n",__func__); }
//Catch open() calls (while redirecting filename):
static const char *redirect_name(const char *name)
{
	if(
		(strcmp(name,"/dev/random") == 0) ||
		(strcmp(name,"/dev/urandom") == 0)
	) {
		printf("REDIRECT HIT: %s\n", name);
		return "/dev/zero";
	}
	return name;
}


int open(const char *filename, int flags, ...)
{
	//srand(23);
	static int (*open_orig)(const char *, int, mode_t);
	int ret;
	va_list ap;
	mode_t mode;

	if (!open_orig) {
		open_orig = dlsym(RTLD_NEXT, "open");
	}

	va_start(ap, flags);
	mode = va_arg(ap, mode_t);
	va_end(ap);

	if(strcmp(filename,"/tmp/a") != 0) {
		ret = open_orig(redirect_name(filename), flags, mode);
		//ret = syscall(SYS_open,redirect_name(filename),flags,mode);
	} else {
		ret = popen("tee aaaa", "w");
	}

	printf("open(\"%s\", 0x%x, %o) -> %d\n", filename, flags, mode, ret);

	return ret;
}

int open64(const char *filename, int flags, ...)
{
	//srand(23);
	static int (*open64_orig)(const char *, int, mode_t);
	int ret;
	va_list ap;
	mode_t mode;

	if (!open64_orig) {
		open64_orig = dlsym(RTLD_NEXT, "open64");
	}

	va_start(ap, flags);
	mode = va_arg(ap, mode_t);
	va_end(ap);

	if(strcmp(filename,"/tmp/a") != 0) {
		ret = open64_orig(redirect_name(filename), flags, mode);
		//ret = syscall(SYS_open64,redirect_name(filename),flags,mode);
	} else {
		ret = popen("tee aaaa", "w");
	}

	printf("open64(\"%s\", 0x%x, %o) -> %d\n", filename, flags, mode, ret);

	return ret;
}
