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

//Catch rand() calls etc...
int rand(void) { i; return 23; }
long int random(void) { i; return 23; }
int rand_r(unsigned int *seedp) { i; return 23; }
int random_r(struct random_data *buf, int32_t *result) { i; return 23; }
//Time
time_t time(time_t *tloc) { i; return (time_t)0; }
int clock_gettime(clockid_t clk_id, struct timespec *tp) { i; tp->tv_nsec=0; tp->tv_sec=0; return (time_t)0; }
//OpenSSL (not working i guess...)
int RAND_bytes(unsigned char *buf, int num) { i; memset(buf, 23, (size_t) num); return 23; }
int RAND_pseudo_bytes(unsigned char *buf, int num) { i; memset(buf, 23, (size_t) num); return 23; }
int FIPS_rand_bytes(unsigned char *buf, int num) { i; memset(buf, 23, (size_t) num); return 23; }
int FIPS_rand_pseudo_bytes(unsigned char *buf, int num) { i; memset(buf, 23, (size_t) num); return 23; }
//IDs
pid_t getpid(void) { i; return (pid_t)0; }
uid_t getuid(void) { i; return (uid_t)0; }
gid_t getgid(void) { i; return (gid_t)0; }
uid_t geteuid(void) { i; return (uid_t)0; }
gid_t getegid(void) { i; return (gid_t)0; }

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

	ret = open_orig(redirect_name(filename), flags, mode);
	//ret = syscall(SYS_open,redirect_name(filename),flags,mode);

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

	ret = open64_orig(redirect_name(filename), flags, mode);
	//ret = syscall(SYS_open64,redirect_name(filename),flags,mode);

	printf("open64(\"%s\", 0x%x, %o) -> %d\n", filename, flags, mode, ret);

	return ret;
}
