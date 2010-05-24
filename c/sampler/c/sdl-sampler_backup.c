#include <stdio.h>
#include <stdlib.h>
#include <termio.h>
#include "SDL.h"
#include "SDL_mixer.h" //Docs: http://www.libsdl.org/projects/SDL_mixer/docs/
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

char gethit() {
	icanon_off();
	char c = getchar();
	tc_reset();
	return c;
}

Mix_Music *play_sound = NULL;
Mix_Chunk *play_sound2 = NULL;
Mix_Chunk *play_sound3 = NULL;

void cleanUp() {
     Mix_FreeMusic(play_sound);
     Mix_FreeChunk(play_sound2);
     Mix_CloseAudio();
     SDL_Quit();
}

char map[] = "hgjfkdlsa" "nbmvcxz" "yturieowpq";

int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_AUDIO); atexit(cleanUp);
    //Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 256);

    //play_sound = Mix_LoadMUS("/home/harvie/Downloads/Shared/Shaggy-Mr._Bombastic.mp3");
    play_sound2 = Mix_LoadWAV("bd.wav");
    play_sound3 = Mix_LoadWAV("s.wav");

    //Mix_PlayMusic(play_sound, 0); //(Mix_Music, loops) -1 = infinite loop, 0 = play 1time
		//Mix_PlayChannel(-1, play_sound2, 9); //(channel, sample, loop)

		//while(Mix_PlayingMusic() || Mix_Playing(-1)) sleep(1);

		puts("Press Q to exit!");
		char c;
		//for(c=0;c<20;c++) Mix_PlayChannel(1, play_sound2, 0);
		icanon_off();
		while(c = getchar()) {
			switch(c) {
				case 'Q': exit(0); break;
				case 'a': case 'A': Mix_PlayChannel(-1, play_sound3, 0); break;
				default: Mix_PlayChannel(-1, play_sound2, 0);
			}
		}

    exit(0);
}

