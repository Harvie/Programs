//BinPatcher 3.0
//by: Harvie 2oo7

/* This code is very useful, if you are cracking software and you need
to share your cracks without whole binary (because of size or copyright).
This is simple way to patch binaries at users home.
Warning: You need to know some things about PE (probably) architecture to use this.
But there is still some automatic computing with addreses,
so you have to know only bit about yours binary...
*/
/*Note: this is patch used to crack free version of CPULower (remove NAG screen),
and you have to edit it for any file, that you want to patch.
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  //Basic informations
  char file[] = "cpulower.exe";
  char this_file[] = "BinPatcher.exe";
  char title[] = "CPU Lower - NAG screen remover - patcher";
  char author[] = "<-Harvie 2oo7";
  //You can get these from debuger, disassembler, etc...:
  long int image_base = 0x400000; //ImageBase of binary
  long int section_rva = 0x1000; //Virtual addres of patched section
  long int section_offset = 0x400; //Offset of patched section
  long int size = 0, needed_size = 317440;  //Size in Bytes (Compressed size for PE compressors)
  
  //File declaration
  FILE *bin;

  //Banner
  printf("%s\nby: %s\n\nThis will patch %s\n\n", title, author, file);
  
  //Arguments
  if(argc != 2) {
    printf("Incorrect number of arguments!!!\n");
    printf("Usage: %s %s\n", this_file, file);
    printf("You can simply Drag&Drop file \"%s\" on this (%s)\n\n", file, this_file);
    system("pause");
    return(0);    
          }
  
  //Size check
  int c;
  printf("Checking file size of %s...\n", argv[1]);
  bin = fopen(argv[1], "rb");
    if(bin == NULL) { printf("Error while opening %s\n", argv[1]); return 0; }
    while( (c = fgetc(bin)) != EOF ) {
      size++;
    }
  fclose(bin);
  printf("File size = %d B\n", size);
  if(size != needed_size) {
            printf("Incorrect file size (%d B) !!!\nContinue?\n", size);
            system("pause");
          } else {
            printf("File size OK!\n\n");       
          }
          
  //Backup
  char backup[1024];
  printf("Backuping to %s.crkbak\n", argv[1]);
  sprintf(backup, "copy /B /Y \"%s\" \"%s.crkbak\" > nul", argv[1], argv[1]);
  printf("Backup done.\n\n");
  system(backup);
  
  //UnCompress example for upx (Use this only if you know what it is)
  /*
  printf("Uncompressing...\n");
  char uncs[1024];
  sprintf(uncs, "upx -d \"%s\"", argv[1]);
  system(uncs); //THis needs upx binary
  printf("Uncompressed!\n\n");
  */
  
  //Patching
  long int virtual_addres = 0x437069; //Virtual addres from debuger - Example: 0x00437069
  unsigned char patch[] = "\x90\x90\x90\x90\x90"; //Patch to apply - Example: "\x90" (== NOP)
  bin = fopen(argv[1], "rb+");
    if(bin == NULL) { printf("Error while opening %s\n", argv[1]); return 0; }
  
  long int offset = virtual_addres - image_base - section_rva + section_offset; //Vypocitame offset v souboru
  printf("Patching code @ D: %d H: 0x%x\n", offset, offset);
  fseek(bin, offset, SEEK_SET); //Seek code
  fwrite(&patch, (sizeof(patch)-1), 1, bin); //Patch code
  
  //GO HOME ;}}
  fclose(bin);
  
  printf("File was successfully patched!!!\n\n");  
  system("PAUSE");	
  return 0;
}
