#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  printf("HEXCMP 0.3\nHexadecimal Comparator\n<-Harvie 2oo7\n\n");
  if(argc < 3) {
          printf("Usage: hexcmp oldfile newfile [decimal_offset_to_start_at]\n");
          printf("This will show what is new in \"newfile\",\n");
          printf("this can be useful, when making binary patchers.\n\n");
          system("pause");
          return 0;
          }
    
  printf("Comparing old \"%s\" with new \"%s\"\n", argv[1], argv[2]); 
  
  FILE *forig, *fcrac;
  forig = fopen(argv[1], "rb");
    if(forig == NULL) { printf("Error while opening %s\n", argv[1]); return 0; }
  fcrac = fopen(argv[2], "rb");
    if(fcrac == NULL) { printf("Error while opening %s\n", argv[2]); return 0; }
  
  short found = 0;
  unsigned long int offset = 0, last = 0, end = 0, dlen = 0;
  int corig = 0, ccrac = 0;
  
  if(argc > 3) {
          offset = atof(argv[3]);
          printf("Comparing from offset D: %d H: 0x%x\n", offset, offset);
          }
  printf("\n");
  
  //Seek
  fseek(forig, offset, SEEK_SET);
  fseek(fcrac, offset, SEEK_SET);
  
  while((ccrac = fgetc(fcrac)) != EOF) {
    if( (corig = fgetc(forig)) == EOF ) {
       corig == -9999;
       if(end == 0) {
         end = 1;   
       }
    }
    if(corig != ccrac) {
      
      if(end == 1) {
        if(dlen != 0) {
                printf("\nLenght: %d Bytes", dlen);
                dlen = 0;
                }
        printf("\n\n--- END OF OLDER FILE ---\n\n"); 
        end = 2;
      }
      
      if(offset != (last+1)) {
        if(dlen != 0) {
          printf("\nLenght: %d Bytes", dlen);
          dlen = 0;
          printf("\n\n");
        }
        printf("Difference @ D: %d H: 0x%x\n", offset, offset);
      }
        
      printf("\\x%x", ccrac);
      
      dlen++;
      last = offset;
      found = 1;
    }
    
    offset++;
  }
  
   if(dlen != 0) {
          printf("\nLenght: %d Bytes", dlen);
        }
  
  fclose(forig);
  fclose(fcrac);
  
  if(found == 0) {
           printf("No difference found!!!");
           }
  printf("\n");
  //system("PAUSE"); //Debug
  return 0;
}
