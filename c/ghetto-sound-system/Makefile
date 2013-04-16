sdl_BIN=sampler synth
alsa_BIN=beatdetect midi-rx

#sdl_CFLAGS+=-lSDL_sound -lSDL -lSDL_mixer
sdl_CFLAGS+=$(shell sdl-config --cflags --libs) -lSDL_mixer
sdl_CFLAGS+=-lm #synth
alsa_CFLAGS+=-lasound

.PHONY: all clean sdl alsa go midi
all: sdl alsa
	echo .gitignore $(sdl_BIN) $(alsa_BIN) ftdi-midi | tr ' ' '\n' > .gitignore
clean:
	rm .gitignore $(sdl_BIN) $(alsa_BIN)

sdl:
	$(MAKE) CFLAGS+="$(sdl_CFLAGS)" $(sdl_BIN)
alsa:
	$(MAKE) CFLAGS="$(alsa_CFLAGS)" $(alsa_BIN)

midi:
	$(MAKE) CFLAGS="-lftdi -lasound" ftdi-midi


go: sampler
	./$? samples/*/* 2>/dev/null
