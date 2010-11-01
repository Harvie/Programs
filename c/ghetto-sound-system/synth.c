#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "SDL.h"
#include "SDL_mixer.h"

#define PI 3.1415
#define deg2rad(a) (((a)/180)*PI)


SDL_AudioSpec *desired, *obtained;
SDL_AudioSpec *hardware_spec;


void my_audio_callback(void *userdata, Uint8 *stream, int len) {
	int sampl_freq = desired->freq;
	sampl_freq = 44100;

	double sine_freq = 50;
	static double i,j; // = 0;

	short int *u; u = (short int *)stream;
	//for(;len>0;len--) *(u++) = random();
	while( (void *)u<(void *)(stream+len) ) {
		// *(u++) = 0;
		*(u++) = sin(
			i += (2*PI*sine_freq)/sampl_freq
		)*128000;
		// *(u++) = sin(deg2rad(j+=8))*1280000;
		// *(u++) = 0;
		*(u++) = *(u-1);
	}
	//if(i>=2*PI) i=0;
	//if(j>=2*PI) j=0;
}


int main(int argc, char *argv[]) {

       /* Allocate a desired SDL_AudioSpec */
       desired=(SDL_AudioSpec *)malloc(sizeof(SDL_AudioSpec));

       /* Allocate space for the obtained SDL_AudioSpec */
       obtained=(SDL_AudioSpec *)malloc(sizeof(SDL_AudioSpec));

       /* 22050Hz - FM Radio quality */
       desired->freq=22050;
       desired->freq=44100;

       /* 16-bit signed audio */
       desired->format=AUDIO_S16LSB;
       //desired->format=AUDIO_S8;

       desired->channels=2; //0,1,2 = autodetect,mono,stereo

       /* Large audio buffer reduces risk of dropouts but increases response time */
       desired->samples=65530;

       /* Our callback function */
       desired->callback=my_audio_callback;

       desired->userdata=NULL;
       desired->userdata=&desired->freq;

       /* Open the audio device */
       if ( SDL_OpenAudio(desired, obtained) < 0 ){
         fprintf(stderr, "Couldn't open audio: %s", SDL_GetError());
         exit(-1);
       }
       /* desired spec is no longer needed */
       free(desired);
       hardware_spec=obtained;

       /* Prepare callback for playing */

       /* Start playing */
       SDL_PauseAudio(0);
			 sleep(100);
}
