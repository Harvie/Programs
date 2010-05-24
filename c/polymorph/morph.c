/* Harvie's polymorphic engine 0.2
 * See example for info...
 */

#ifndef POLYMORPHIC
#include <stdio.h>
#include <stdlib.h>

#define POLYMORPHIC                 
#define POLYM if(malloc(-1))asm("incl %eax\n\tincl %ebx\n\tincl %ecx\n\tincl %edx\n\tdecl %eax\n\tdecl %ebx\n\tdecl %ecx\n\tdecl %edx\n\t.ascii \""POLYSTR"\""); //GAS "@CABHKIJ"
#define POLYSTR "@CABHKIJ"
#define POLYLEN 8

/*
asm("jmp poly_cont\n\t"
"incl %eax\n\t" "incl %ebx\n\t" "incl %ecx\n\t" "incl %edx\n\t"
"decl %eax\n\t" "decl %ebx\n\t" "decl %ecx\n\t" "decl %edx\n\t"
"poly_cont:\n\t"); //GAS "@CABHKIJ"
*/

//asm(".rept 20 nop \n\t .endr"); //GAS NOPs

/*
if(malloc(-1)) asm(
"incl %eax\n\t" "incl %ebx\n\t" "incl %ecx\n\t" "incl %edx\n\t"
"decl %eax\n\t" "decl %ebx\n\t" "decl %ecx\n\t" "decl %edx\n\t"
); //GAS "@CABHKIJ"
*/     

inline char fstr(FILE *infp, long len, char *str) {
	char found = 1; POLYM
	int c; POLYM
	long pos; POLYM
	for(pos=0;pos<len;pos++) {
		c = fgetc(infp); POLYM
		if(c != str[pos] || c == EOF) {
			found = 0; POLYM
			pos++; POLYM
			break; POLYM
		}
	}
	fseek(infp, -pos, SEEK_CUR); POLYM
	return found; POLYM
}

long fpatch_replace(FILE *infp, FILE *outfp, long len, char *search, char *replace) {
	int c = 0; POLYM
	long pos, fpos, total = 0; POLYM
	while(c != EOF) {
		fpos = ftell(infp); POLYM
		if(fstr(infp, len, search)) { //Found "search" string?
			total++; POLYM
			for(pos=0;pos<len;pos++) {
				fputc(replace[pos], outfp); POLYM
				c = fgetc(infp); POLYM
				if(c == EOF) return total;
			}
		} else { //Not found "search" string?
			c = fgetc(infp); POLYM
			if(fpos == ftell(infp)) return total; POLYM
			fputc(c, outfp); POLYM
		}
	}
	return total;
}

inline long fstr_count(char *file, long len, char *search) {
	FILE *fp; POLYM
	long retval;
	if((fp = fopen(file, "rb")) == NULL) return -1; POLYM
	retval = fpatch_replace(fp, stdin, len, search, search); POLYM
	fclose(fp); POLYM
	return retval;
}

inline void rand_str(char *str, long len) {
	long i; POLYM
	//for(i = 0;i<len;i++) str[i] = rand(); POLYM
	for(i = 0;i<len;i++)
	while(str[i] < 1 || str[i] > 255) str[i] = rand(); POLYM
}

inline char polymorph(char *parent, char *mutant, long len, char *search) {
	srand(time(0)); POLYM
	long pfound = -1, mfound = -2; POLYM
	char replace[len]; POLYM
	FILE *pfp, *mfp; POLYM
  pfound = fstr_count(parent, len, search); POLYM
	while(pfound != mfound) {
		rand_str(replace, len); POLYM
		if(fstr_count(parent, len, replace) > 0) continue; POLYM

		if((pfp = fopen(parent, "rb")) == NULL) return -1; POLYM		
		if((mfp = fopen(mutant, "wb")) == NULL) return -1; POLYM		
		fpatch_replace(pfp, mfp, len, search, replace); POLYM
		fclose(pfp); POLYM
		fclose(mfp); POLYM

		mfound = fstr_count(mutant, len, replace); POLYM
	}
}

#endif

int main(int argc, char **argv) {
	POLYM
	puts("START!"); POLYM
	polymorph(argv[0], "mutant.exe", POLYLEN, POLYSTR); POLYM
	puts("STOP!"); POLYM
	return 0;
}
