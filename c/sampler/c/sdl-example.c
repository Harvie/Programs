#include <stdio.h>
#include <termio.h>
#include "SDL.h"
#include "SDL_mixer.h" //Docs: http://www.libsdl.org/projects/SDL_mixer/docs/
//example http://www.kekkai.org/roger/sdl/mixer/

static struct termios stored_settings;
void icanon_on() {
    struct termios new_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);
}

void icanon_off() {
    tcsetattr(0,TCSANOW,&stored_settings);
}

char gethit() {
	icanon_on();
	char c = getchar();
	icanon_off();
	return c;
}

Mix_Music *play_sound = NULL;
Mix_Chunk *play_sound2 = NULL;

void cleanUp() {
     Mix_FreeMusic(play_sound);
     Mix_FreeChunk(play_sound2);
     Mix_CloseAudio();
     SDL_Quit();
}


int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

    play_sound = Mix_LoadMUS("/home/harvie/Downloads/Shared/Shaggy-Mr._Bombastic.mp3");
    play_sound2 = Mix_LoadWAV("bd.wav");

    Mix_PlayMusic(play_sound, 0); //(Mix_Music, loops) -1 = infinite loop, 0 = play 1time
		Mix_PlayChannel(-1, play_sound2, 9);

		//while(Mix_PlayingMusic() || Mix_Playing(-1)) sleep(1);
		puts("press any key to exit!"); gethit();


    cleanUp();
    return 0;
}

