#include <stdio.h>
#include <fcntl.h>
#include <asm-generic/ioctls.h>
#include <unistd.h>

/* Pulse DTR (Harie 2o1o)
 * This can be used eg.: for reseting Arduino
 *
 * make serial-pulse-dtr
 * ./serial-pulse-dtr /dev/ttyUSB0
 *
 * (-; peace ;-)
 */

int main(int argc, char *argv[]) {
	if(argc < 2) printf("Usage: %s [/dev/tty*]\n", argv[0]);

	int fd = open(argv[1], O_WRONLY);
	perror(argv[0]);

	//DTR HIGH
	//ioctl(fd, TIOCSDTR);
	ioctl(fd, TIOCMBIS);

	//Wait
	//system("sleep 0.1");
	usleep(100000);

	//DTR LOW
	//ioctl(fd, TIOCCDTR);
	ioctl(fd, TIOCMBIC);

	close(fd);
	return 0;
}
