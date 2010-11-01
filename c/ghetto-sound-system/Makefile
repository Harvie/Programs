#CFLAGS=-lSDL_sound -lSDL -lSDL_mixer
CFLAGS+=`sdl-config --cflags --libs` -lSDL_mixer
CFLAGS+=-lm

.PHONY: all go
all: sampler synth
go: sampler
	./$? samples/*/* 2>/dev/null
