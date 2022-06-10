//gcc -Wextra -pedantic-errors -o main.out main.c -lSDL2

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>


void SDL_AudioCallbacks(void* userdata, Uint8* stream, int len) {
	printf("%d\n", (int)*((int *)stream) );
}

int main(void) {
	SDL_AudioSpec want, have;
	SDL_AudioDeviceID dev;

	SDL_memset(&want, 0, sizeof(want)); /* or SDL_zero(want) */
	want.freq = 48000;
	want.format = AUDIO_F32;
	want.channels = 2; //1,2,4,6
	want.samples = 4096;
	want.callback = SDL_AudioCallbacks; /* you wrote this function elsewhere -- see SDL_AudioSpec for details */
	//want.userdata = NULL;

	if((SDL_Init(SDL_INIT_AUDIO)==-1)) {
        	printf("Could not initialize SDL: %s.\n", SDL_GetError());
        	exit(-1);
    	}

	int i, count = SDL_GetNumAudioDevices(0);
	for (i = 0; i < count; ++i) {
    		printf("Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
	}

	dev = SDL_OpenAudioDevice(NULL, 1, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
	if (dev == 0) {
		SDL_Log("Failed to open audio: %s", SDL_GetError());
	} else {
		if (have.format != want.format) { /* we let this one thing change. */
			SDL_Log("We didn't get Float32 audio format.");
		}
		SDL_PauseAudioDevice(dev, 0); /* start audio playing. */
		SDL_Delay(5000); /* let the audio callback play some sound for 5 seconds. */
		SDL_CloseAudioDevice(dev);
	}

    return EXIT_SUCCESS;
}
