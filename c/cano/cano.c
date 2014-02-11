#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/fcntl.h>

#include <dlfcn.h>
//#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/stat.h>

int stat(const char *path, struct stat *buf) {
	buf->st_size = (off_t) 5;
	return 0;
}

int pipe_read() {
	return fileno(popen("xxd -r /tmp/b", "r"));
}
int pipe_write() {
	return fileno(popen("read -n 1 i || exit; (echo -n \"$i\"; cat) | xxd > /tmp/b", "w"));
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

	if(strcmp(filename,"/tmp/a") != 0) {
		ret = open_orig(filename, flags, mode);
		//ret = syscall(SYS_open,redirect_name(filename),flags,mode);
	} else {
		//ret = fileno(popen("echo aaaa", "rb"));
		if((flags & O_WRONLY) || (flags & O_RDWR)) {
			ret = pipe_write();
		} else {
			ret = pipe_read();
		}
		//ssize_t write(int fd, const void *buf, size_t count);
	}

	//printf("open(\"%s\", 0x%x, %o) -> %d\n", filename, flags, mode, ret);

	return ret;
}

//hackystuff, fixme:
int open64(const char *filename, int flags, ...) {
	va_list ap;
	mode_t mode;

	va_start(ap, flags);
	mode = va_arg(ap, mode_t);
	va_end(ap);

	return open(filename, flags, mode);
}

