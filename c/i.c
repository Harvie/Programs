// BruteForCer - A combination sequence generator
// Harvie 2oo7
/*
 * I hope, that this can show you that BF is the most suxing idea.
 * You can use GREP pipe, if you know part of password.
 * Brutforcing is the only way to be the real hacker. (Please don't pwn me ;)
 * More info? UTFS!
 */

#include <stdio.h>

void inc(char *comb, int i, int maxno) {
	if (comb[i] == -1) {
		comb[i] = 0;
		return;
	}
	comb[i] = (comb[i] + 1) % maxno;
	if (comb[i] == 0)
		inc(comb, i + 1, maxno);
}

int main() {
	int i, j;
	int minlen = 0;
	int maxlen = 10;	// Lenght of string
	// char full = 0; //0=Alphabet, 1=All possibilities
	// char alpha[] = "01";
	char alpha[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ";	// Alphabet
	int alphmax = sizeof(alpha) - 1;	// printf("%d\n",
	// alphmax);

	char comb[maxlen];
	for (i = 0; i <= maxlen; i++) {
		comb[i] = -1;
		if (i < minlen)
			comb[i] = 0;
	}
	while (comb[maxlen] == -1) {
		for (i = maxlen - 1; i >= 0; i--)
			if (comb[i] != -1)
				putchar(alpha[comb[i]]);
		putchar('\n');
		inc(comb, 0, alphmax);
	}

	return 0;
}
