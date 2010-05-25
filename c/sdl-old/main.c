#include <stdlib.h>
#include <SDL.h>
#include <SDL_mixer.h>

const max_sounds=50;
Mix_Chunk *sounds[max_sounds];
Mix_Music *music;

//kanaly
enum {
SNDT_0,
SNDT_1,
SNDT_2,
SNDT_3,
SNDT_4,
SNDT_5,
SNDT_6,
SNDT_7
};

//zvuky
enum {
SND_ZVUK,
SND_NUM
};

int main(int argc,char *argv[])
{
if ( SDL_Init(SDL_INIT_AUDIO) < 0 ) {
    fprintf(stderr, "Problem: %s\n", SDL_GetError());
    exit(1); 
}
atexit(SDL_Quit);

//frekvence 22050 Hz
//MIX_DEFAULT_FORMAT - Signed 16-bit zvuky, v systemovem bytovem poradi (little,big endian)
//mono-stereo: pro mono vlozte 1, pro stereo 2

if (Mix_OpenAudio(22050/*frekvence [Hz]*/, MIX_DEFAULT_FORMAT, 2/*mono-stereo*/,1024/*velikost bloku*/)==-1)
{
    fprintf(stderr,"Mix_OpenAudio: %s\n", Mix_GetError());
    exit(1);
}

 sounds[SND_ZVUK] = Mix_LoadWAV("zvuk.wav");
if (sounds[SND_ZVUK]==NULL) 
{
    fprintf(stderr,"zvuk [zvuk.wav] nebyl nahran\n");exit(2);}

 //kanal: -1 pro prvni volny kanal
//pocet opakovani x: zvuk se bude opakovat x+1 krat,x=-1 pro stale opakovani.
Mix_PlayChannel(-1/*kanal*/,sounds[SND_ZVUK]/*Mix_Chunk*/,0/*pocet opakovani*/);

 music=Mix_LoadMUS("music.mp3");
if (!music) {
    printf("Mix_LoadMUS(\"music.mp3\"): %s\n", Mix_GetError());
    exit(2);
}

//pocet opakovani x: hudba se bude opakovat x krat,x=-1 pro stale opakovani.
//postupne zvysuje hlasitost hudby od 0 do 100% za 2000 ms po spusteni

Mix_FadeInMusic(music, -1/*pocet opakovani*/,2000 /*cas[ms]*/);

//lze i jen provest pouhe spusteni hudby
//Mix_PlayMusic(music, -1/*pocet opakovani*/);

SDL_Delay(5000);

 //ztlumi hudbu az do ticha a vypne
Mix_FadeOutMusic(2000);

SDL_Delay(1000);

Mix_CloseAudio();
return 0;
}