//#define _GNU_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MEM_PATH "hello.bin"
#define MEM_SIZE 1048576

int main() {
	int fd;
	fd = open(MEM_PATH, O_RDWR | O_SYNC | O_CREAT | O_CLOEXEC, S_IRUSR | S_IWUSR);
	ftruncate(fd, MEM_SIZE);

	void *mem;
	mem = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED , fd, 0);
	close(fd);

	char hello[] = "Hello!";
	memcpy(mem, hello, sizeof(hello));
	msync(mem, sizeof(hello), MS_SYNC);
}
