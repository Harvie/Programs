#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <stdarg.h>

int rand(void) { 	return 23; }
int random(void) { 	return 23; }

static const char *redirect_name(const char *name)
{
	if(strcmp(name,"/dev/random") == 0) return "/dev/zero";
	if(strcmp(name,"/dev/urandom") == 0) return "/dev/zero";
	return name;
}

int open(const char *filename, int flags, ...)
{
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

	ret = open_orig(redirect_name(filename), flags, mode);

	printf("open(\"%s\", 0x%x, %o) -> %d\n", filename, flags, mode, ret);

	return ret;
}
