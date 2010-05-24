PROJ=sdl
#CFLAGS=-lSDL_sound -lSDL -lSDL_mixer
CFLAGS+=`sdl-config --cflags --libs` -lSDL_mixer
CFLAGS+=-lm
$(PROJ): $(PROJ).c
go: $(PROJ)
	./$? drums/* voice/* loops/* 2>/dev/null
