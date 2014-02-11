#define _GNU_SOURCE
/*
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
*/
#include <dlfcn.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <linux/limits.h>

#define i { srand(23); fprintf(stderr,"%s();\n",__func__); }
//Catch open() calls (while redirecting filename):

static const char *fd_to_path(int fd) {
	static char path[PATH_MAX];
	sprintf(path,"/proc/self/fd/%d",fd);
	int l = (int) readlink((char * __restrict__) path, path, PATH_MAX);
	path[l]=0;
	return path;
}

int close(int fd) {
	//fputs(fd_to_path(fd),stderr);
	fprintf(stderr,"%s(%d,%s);\n",__func__,fd,fd_to_path(fd));
	if(fd>2) return syscall(SYS_close,fd);
}
int fclose(FILE *fd) {
	//fputs(fd_to_path(fd),stderr);
	fprintf(stderr,"%s(%d,%d,%s);\n",__func__,fileno(fd),fd,fd_to_path(fileno(fd)));
	static ssize_t (*fclose_orig)(FILE *fd);
	if (!fclose_orig) {
		fclose_orig = dlsym(RTLD_NEXT, "fclose");
	}
	return fclose_orig(fd);
	//return syscall(SYS_close,fileno(fd));
}
/*
ssize_t write(int fd, const void *buf, size_t count) {
	//fputs(fd_to_path(fd),stderr);
	printf("%d",fd);
	i;
	return syscall(SYS_write,fd,buf,count);
}
*/
/*
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

	if(strcmp(filename,"/tmp/lol.txt.asc") != 0) {
		ret = open_orig(redirect_name(filename), flags, mode);
		//ret = syscall(SYS_open,redirect_name(filename),flags,mode);
	} else {
		if(flags & O_RDONLY) {
			ret = fileno(popen("cat aaaa", "r"));
		} else {
			ret = fileno(popen("tee aaaa", "w"));
		}
	}

	printf("open(\"%s\", 0x%x, %o) -> %d\n", filename, flags, mode, ret);

	return ret;
}

*/
