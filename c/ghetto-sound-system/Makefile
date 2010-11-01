sdl_BIN=sampler synth
alsa_BIN=beatdetect

#sdl_CFLAGS+=-lSDL_sound -lSDL -lSDL_mixer
sdl_CFLAGS+=$(shell sdl-config --cflags --libs) -lSDL_mixer
sdl_CFLAGS+=-lm #synth
alsa_CFLAGS+=-lasound

.PHONY: all clean sdl alsa go
all: sdl alsa
	echo .gitignore $(sdl_BIN) $(alsa_BIN) | tr ' ' '\n' > .gitignore
clean:
	rm .gitignore $(sdl_BIN) $(alsa_BIN)

sdl:
	$(MAKE) CFLAGS+="$(sdl_CFLAGS)" $(sdl_BIN)
alsa:
	$(MAKE) CFLAGS="$(alsa_CFLAGS)" $(alsa_BIN)


go: sampler
	./$? samples/*/* 2>/dev/null
