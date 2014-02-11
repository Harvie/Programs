#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <dlfcn.h>
//#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
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


int pipe_read() {
       /*
	int pipefd[2];
	pipe(pipefd);
	if(!fork()) {
		//write(pipefd[1],"ahoj\n",5);
		syscall(SYS_write,pipefd[1],"ahoj\n",5);
		close(pipefd[1]);
		exit(0);
	}
	return pipefd[0];
       */
	return dup(fileno(popen("echo lolpajp", "r")));
}
int fdd;
ssize_t read(int fd, void *buf, size_t count) {
	fprintf(stderr,"%s(%d,%d,%s);\n",__func__,fd,count,fd_to_path(fd));
	if(!fdd) fdd=pipe_read();
	if(strcmp(fd_to_path(fd),"/usr/share/terminfo/x/xterm")!=0) {
		return syscall(SYS_read,fdd,buf,count);
	} else {
		return syscall(SYS_read,fd,buf,count);
	}
}
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	fprintf(stderr,"%s(%d,%d);\n",__func__,stream,fileno(stream));
	return read(fileno(stream),ptr,size*nmemb);
}
/*
ssize_t write(int fd, void *buf, size_t count) {
	fprintf(stderr,"%s(%d,%d);\n",__func__,fd,count);
	return syscall(SYS_write,fd,buf,count);
}
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
	return write(fileno(stream),(void *)ptr,size*nmemb);
}
*/
/*
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
		//ret = fileno(popen("cat aaaa", "r"));
		int pipefd[2];
		pipe(pipefd);
		ret = pipefd[0];
		//ssize_t write(int fd, const void *buf, size_t count);
		if(!fork()) { write(pipefd[1],"ahoj\n",5); close(pipefd[1]); exit(0); }
	}

	//printf("open(\"%s\", 0x%x, %o) -> %d\n", filename, flags, mode, ret);

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
		//ret = fileno(popen("cat aaaa", "r"));
		int pipefd[2];
		pipe(pipefd);
		ret = pipefd[0];
		//ssize_t write(int fd, const void *buf, size_t count);
		if(!fork()) { write(pipefd[1],"ahoj\n",5); close(pipefd[1]); exit(0); }
	}

	//printf("open64(\"%s\", 0x%x, %o) -> %d\n", filename, flags, mode, ret);

	return ret;
}
*/
