#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int r;
  float i;
  i = 1024; //Cycles
  if(argc > 1) {
    if( strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "/?") == 0 ) {
        printf("Usage: keygen [X] [> key.txt]\n");
        printf("keygen - generates 1024 random chars (bytes) to stdin\n");
        printf("keygen X - generates X random chars (bytes) to stdin\n");
        printf("keygen [X] > key.txt - generates X random chars (bytes) to key.txt\n");
        } 
    i = atof(argv[1]);
    }
  srand(time(0));
  while(i>0) {
    r = rand();
    putchar(r);
    //sleep(500);
    i--;
  }
  //system("PAUSE");	
  return 0;
}
