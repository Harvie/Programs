/*
Add SDL.lib, SDLmain.lib and SDL_mixer.lib to your project settings. 
g++ -o sdl02 sdl.c `sdl-config --cflags --libs`

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

class sound{
public:
sound();
~sound();
void playWAV(const char *wav);
void playMP3(const char *mp3);
};

sound::sound(){}
sound::~sound(){}

void sound::playWAV(const char *wav){
    Mix_Chunk *music;
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
    music = Mix_LoadWAV(wav);
    Mix_PlayChannel(1,music,0);
}

void sound::playMP3(const char *mp3){
    Mix_Music *music;
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
    music = Mix_LoadMUS(mp3);
    Mix_VolumeMusic(100);
    Mix_PlayMusic(music,-1);

}

