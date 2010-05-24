#include <stdio.h>
int main(){
int *d=malloc(sizeof(int));

//d=&d;
const int x=3; //x=4;
d=&x;

*d=5;
printf("%d\n",*d); //overeni
return 0;
}
