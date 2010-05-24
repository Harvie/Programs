#include <stdio.h>

void shellcode() {
	//Enter code here
	char lol[]="fdsjkgfdskhgjdshgjdshkgdhskjghdsjkghdskhj\n";
	system("ls");
}

int main() {
	//printf("Shellcode size: %d bytes\n", sizeof(&shellcode));
	int i;
	for(i=0; i<=256; i++) putchar((char *)(shellcode+i));
	shellcode();
}
