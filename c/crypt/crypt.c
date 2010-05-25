#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int k, i; //Key, In,
  //char sfkey[128] sfin[128], sfout[128];
  if (argc <= 1 || argc > 4 ) { printf("Usage: crypt key [in] [out]\n"); return 1; }
  FILE* fkey = fopen(argv[1], "rb"); //Open Key
  if (argc >= 3) { freopen(argv[2], "rb", stdin); } //Reopen stdin
  if (argc == 4) { freopen(argv[3], "wb", stdout); } //Reopen stdout
  
  while( (i = getchar()) != EOF ) {
           if ( (k = getc(fkey)) == EOF )
              { fseek(fkey, 0L, SEEK_SET); k = getc(fkey); }
             //printf("%d: %c\n", k, k); sleep(1); //Debug
             putchar(i^k);
  }
  return 0;
}
