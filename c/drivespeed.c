#include <stdio.h>
#include <stdlib.h>

int main() {

char drive[] = "/dev/hdc";
int stime = time(NULL);
printf("%d\n", stime);
system("cat /dev/hdc > /dev/null");
stime = time(NULL);
printf("%d\n", stime);
return(0);

}
