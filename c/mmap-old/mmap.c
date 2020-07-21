#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

unsigned char *fmmap(FILE *fd) {
	struct stat sb;

	if(!fd) perror("");
	fd = fileno(fd);
	fstat((int)fd, &sb);
	return mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE, (int)fd, 0);
}

int main() {
	FILE *fd;
	unsigned char *mm;

	fd = fopen("mmap.c", "r");
	mm = fmmap(fd);

	perror("Status");
	puts(mm);

	//munmap(mm, 10);
	fclose(fd);
}
