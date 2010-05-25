#include <stdio.h>
#include <math.h>
#include <time.h>
#define PI 3.141592653589793

int main(void) {
  double currPi=0, i=1;
  clock_t begin, end;

  begin = clock();
  do {
    currPi+=(4/i - 4/(i+2));
    i+=4;
  } while(fabs(PI-currPi) > 1e-7);
  end = clock();

  printf("Vypocteno Pi = %lf v case: %f ms.\n", currPi,
         (float)(end-begin)/CLOCKS_PER_SEC * 1000);
         
    //system("PAUSE");
    //return EXIT_SUCCESS;
}
