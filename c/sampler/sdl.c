/* Ghetto Sound - Sampler
 * by: Harvie 2o1o
 *
 * TODO:
 * - port unbuffered getchar to windows
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termio.h>
#include "SDL.h"
#include "SDL_mixer.h"
//Docs: http://www.libsdl.org/projects/SDL_mixer/docs/
//example http://www.kekkai.org/roger/sdl/mixer/

static struct termios stored_settings;
void tc_reset() { tcsetattr(0,TCSANOW,&stored_settings); }
void icanon_off() {
	struct termios new_settings;
	tcgetattr(0,&stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_lflag &= (~ECHO);
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&new_settings);
	atexit(tc_reset);
}

/*char gethit() {
	icanon_off();
	char c = getchar();
	tc_reset();
	return c;
}*/

Mix_Chunk **sample = NULL;
int samplec = 0;

void cleanUp() {
	int i;
	for(i=0;i<samplec;i++) { Mix_FreeChunk(sample[i]); }
	Mix_CloseAudio();
	SDL_Quit();
}

char *keymap = "xcvbnmasdfghjklwertuiop789456123zy";
int i;
int main(int argc, char* argv[]) {
	if(argc == 1) {
		printf("\n\tUsage:\t%s *.wav *.ogg [...]\n", argv[0]);
		printf("\t\t%s --keys 7894561230 *.wav *.ogg [...] #to use own key-bindings\n", argv[0]);
		puts("\t\t- lowercase: play sample once; uppercase: loop sample;");
		puts("");
		puts("\t- currently supported formats: WAVE, AIFF, RIFF, OGG(libvorbis), VOC, MOD(libmikmod), MIDI, MP3(smpeg)");
		puts("\t- currently there are issues with mp3s - you should convert it:");
			puts("\t\t- mp3 to wav: ffmpeg -i in.mp3 out.wav");
			puts("\t\t- mp3 to ogg: ffmpeg -i in.mp3 -acodec vorbis -ac 2 -aq 60 out.ogg");
		puts("");
		exit(-1);
	}

	//Init SDL:
	if(SDL_Init(SDL_INIT_AUDIO)) {
		perror("Cannot initialize SDL using SDL_Init!\n");
		exit(1);
	}
	atexit(cleanUp);
	//Open audio
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 256)) { //sampl.freq(def22050), format, 2stereo/1mono, chunksize(default 4095)
		perror("Cannot open audio using Mix_OpenAudio!\n");
		exit(2);
	}
	//alloc enough mixer channels
	Mix_AllocateChannels(argc*2);
	//set volume
	Mix_Volume(-1,MIX_MAX_VOLUME/2);

	//alloc enough samples
	sample = malloc(sizeof(Mix_Chunk)*argc);
	samplec = 0;


	i = 1;
	argv++; argc--; //get rid of argv[0]
	while(argc) { //loop through args (load samples and parse options)
		if(!strcmp(argv[0], "--keys")) { //alternative keymap
			if(samplec > 0) {
				puts("\nERR: Cannot specify keys after loading first sample!");
				break;
			}
			if(--argc) { argv++; }
				else { break; }
			keymap = argv[0];
			argv++; argc--; continue;
		}
		if(samplec < strlen(keymap)) {
			sample[samplec] = Mix_LoadWAV(argv[0]);
			if(sample[samplec]) {
				printf("[%c]: %s%c", keymap[samplec], argv[0], (samplec+1)%4==0?'\n':'\t');
				samplec++;
			} else {
				printf("\nERR: Mix_LoadWAV(%s): %s\n", argv[0], Mix_GetError());
			}
		} else {
			if(i) puts("\nERR: Not enough keys on keyboard (or in keymap)!");
			i = 0;
		}

		argv++; argc--;
	}
	puts("");

	//Main loop:
	puts("[Q]: exit!\t[SPACE]: silence!\t[ENTER]: delay\t[-]: halucination");
	int keymaplen = strlen(keymap);
	//freopen("test.seq", "r", stdin);

	icanon_off();
	char c;	while(c = getchar()) {
		//lowercase: play sample once
		for(i=0;i<keymaplen && i < samplec;i++) if(c == keymap[i]) {
			if(Mix_PlayChannel(samplec+i, sample[i], 0) == -1) printf("ERR: Mix_PlayChannel(sample #%d): %s\n", i, Mix_GetError());
		}
		//uppercase: start/stop loop
		for(i=0;i<keymaplen && i < samplec;i++) if(c == keymap[i]-'a'+'A') {
			if(Mix_Playing(i)) {
				Mix_HaltChannel(i);
			} else {
				if(Mix_PlayChannel(i, sample[i], -1) == -1) printf("ERR: Mix_PlayChannel(sample #%d, loop): %s\n", i, Mix_GetError());
			}
		}
		if(c == ' ') Mix_HaltChannel(-1);
		if(c == '\n') sleep(1);
		if(c == '#') while(getchar() != '\n');
		if(c == '?') while(Mix_Playing(-1)) usleep(1000);
		if(c == '}') rewind(stdin);
		if(c == 'Q' || c == 'q') break;
		if(c == '-') for(i=360;i>=0;i--) { //halucination
			Mix_SetPosition(MIX_CHANNEL_POST, i, 0);
			usleep(2000);
		}
	}

	exit(0);
}

