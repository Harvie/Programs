//BruteForCer - A permutation sequence generator
//Harvie 2oo7
/*
 * I hope, that this can show you that BF is the most suxing idea.
 * You can use GREP pipe, if you know part of password. 
 * Brutforcing is the only way to be the real hacker. (Please don't pwn me ;)
 * More info? UTFS!
 */

#include <stdio.h>

int main() {

	int minlen = 4;
	int maxlen = 4; //Lenght of string
	char full = 0; //0=Alphabet, 1=All possibilities
        unsigned char alpha[] = "1234"; //"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 "; //Alphabet

	unsigned int alphmax = sizeof(alpha)-1;
	char loop;
	int i, pos, len;
	unsigned char comb[maxlen];


	for(len=minlen;len<=maxlen;len++) {


	//Fill string with NULL bytes
  	for(i=0;i<len;i++) {
  		comb[i] = 0;
		}

	loop = 1;
	while(loop) {
	
		//Make next string
		pos = 0;
		comb[pos]++; if(!full && comb[pos]>=alphmax) comb[pos] = 0;
		while(comb[pos]==0) {
			pos++;
			if(pos >= len) loop = 0;
			comb[pos]++; if(!full && comb[pos]>=alphmax) comb[pos] = 0;
		}

		//Print string in reversed order
		for(i=(len-1);i>=0;i--) {
                	if(full) {
				putc(comb[i], stdout);
			} else {
				if(comb[i]<alphmax && alpha[comb[i]] != 0) putc(alpha[comb[i]], stdout);
			}
	        }
		putc('\n', stdout);
	}


	}

}
