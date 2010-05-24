// g++ -std=c99 -lm $(sdl-config --cflags --libs) synth.cpp -o synth

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <SDL/SDL.h> 
#include <SDL/SDL_audio.h>
#include <stdlib.h> 
#include <math.h> 

SDL_Surface *screen; 
SDL_AudioSpec spec; 
Uint32 sound_len; 
Uint8 *sound_buffer; 
int sound_pos = 0; 
int counter; 

//Do all the init stuff 
void init_sdl (void) 
{ 
        if (SDL_Init (SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) 
                exit (-1); 
        atexit (SDL_Quit); 
        screen = SDL_SetVideoMode (640, 480, 16, SDL_HWSURFACE); 
        if (screen == NULL) 
                exit (-1); 
} 

//Creates the sine wave. To supress ripple, the wave runs continuously by using an incemental counter 
void Callback (void *userdata, Uint8 *stream, int len) 
{ 
        double pi = 3.1415; 
        Uint8 *waveptr; 
        double Hz=50;	
        double L = 512; 
        double A = 100; 
        double SR = 44100; 
        double F=2*pi*Hz/SR; 

        for (int z = 0; z< 512 ; z++) 
        { 
                counter++; 
                sound_buffer[z] = (Uint8) A*sin(F*(double)counter); 
        } 

        //Do the sound loop... 
        if (sound_pos + len > sound_len) 
        {	
                sound_pos=0; 
        } 

        waveptr = sound_buffer + sound_pos; 
        SDL_MixAudio(stream, waveptr, len, SDL_MIX_MAXVOLUME); 
        
        //stream = waveptr; //Replacing the mixer gives nothing but silence...!?! 
                
        sound_pos += len; 
} 

void play (void) 
{ 
        sound_buffer = new Uint8[512]; 
        sound_len= 512; 
        spec.freq = 44100; 
        spec.format = AUDIO_S16SYS; 
        spec.channels = 1; 
        spec.silence = 0; 
        spec.samples = 512; 
        spec.padding = 0; 
        spec.size = 0;     
        spec.userdata = 0; 

        spec.callback = Callback; 
        if (SDL_OpenAudio (&spec, NULL) < 0) 
        { 
                printf ("Kann audio nicht öffnen: %s\n", SDL_GetError ()); 
                exit (-1); 
        } 
        SDL_PauseAudio (0); 
} 

int main(int argc, char* argv[]) 
{ 
        init_sdl (); 
        play (); 
        SDL_Delay (5000); 
        return 0; 

}

