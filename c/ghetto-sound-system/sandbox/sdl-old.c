/***************************************************************************
 *   Copyright (C) 2005 by Michal Turek - Woq                              *
 *   WOQ (zavinac) seznam.cz                                               *
 *                                                                         *
 *   Program ukazuje nahrani zvuku ve formatu .AU pomoci knihovny          *
 *   SDL_sound, jeho dekodovani a nasledne prehravani                      *
 *   (opet pro jednoduchost smycka). Zde je pouzit .AU, ale naprosto       *
 *   stejnym zpusobem lze pouzivat zvukove soubory jakychkoli jinych       *
 *   formatu (.MP3, .OGG atd.), staci jen zmenit jmeno souboru.            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_sound.h>// Nezapomenout prilinkovat -lSDL_sound


/*
 * Symbolicke konstanty
 */

#define SDL_SUBSYSTEMS SDL_INIT_AUDIO

/*
 * Funkcni prototypy
 */

void AudioCallback(void *unused, Uint8 *stream, int len);
int Init();					// Inicializace
void Destroy();					// Deinicializace
int ProcessEvent();				// Osetruje udalosti
int  main(int argc, char *argv[]);		// Vstup do programu


/*
 * Globalni promenne
 */


Sound_Sample *g_sample;				// Zvuk
Uint32 g_pos;					// Pozice pri prehravani


/*
 * Audio callback funkce, posila data do streamu
 */

void AudioCallback(void *unused, Uint8 *stream, int len)
{
	// Ukazatel na cast, kde se ma zacit prehravat
	Uint8 *wave_ptr = (Uint8 *)g_sample->buffer + g_pos;

	// Delka zvuku do konce
	int wave_left = g_sample->buffer_size - g_pos;

	// Zbyvajici delka je mensi nez pozadovana
	// Cyklus, protoze cely zvuk muze byt kratsi
	while(wave_left <= len)
	{
		// Posle data na zvukovou kartu
		SDL_MixAudio(stream, wave_ptr, wave_left, SDL_MIX_MAXVOLUME);

		// Posune se o prave zapsana data
		stream += wave_left;
		len -= wave_left;

		// Od zacatku zvuku
		wave_ptr = (Uint8 *)g_sample->buffer;
		wave_left = g_sample->buffer_size;
		g_pos = 0;
	}

	// Je jistota, ze zbyvajici cast zvuku je delsi nez pozadovana
	SDL_MixAudio(stream, wave_ptr, len, SDL_MIX_MAXVOLUME);
	g_pos += len;
}


/*
 * Inicializacni funkce
 */

int Init()
{
	// Inicializace SDL
	if(SDL_Init(SDL_SUBSYSTEMS) == -1)
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n",
				SDL_GetError());
		return 0;
	}

	// Inicializace SDL_sound
	if(Sound_Init() == 0)
	{
		fprintf(stderr, "Unable to initialize SDL_sound: %s\n",
				Sound_GetError());
		return 0;
	}

	// Vypise verzi SDL_sound
	Sound_Version compiled;
	Sound_Version linked;
	SOUND_VERSION(&compiled);
	Sound_GetLinkedVersion(&linked);

	printf("\nVersion of compiled SDL_sound: %d.%d.%d\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("Version of linked SDL_sound: %d.%d.%d\n\n",
		linked.major, linked.minor, linked.patch);

	// Vypise dostupne audio formaty
	const Sound_DecoderInfo **dec_info = Sound_AvailableDecoders();

	printf("Supported sound formats:\n");
	int i;
	for(i = 0; dec_info[i] != NULL; i++)
	{
		printf("%s\t- %s\n",
			*dec_info[i]->extensions, dec_info[i]->description);
	}
	printf("\n");

	// Nastaveni audia
	SDL_AudioSpec desired, obtained;
	desired.freq = 22050;// 16-bit. stereo na 22 kHz
	desired.format = AUDIO_S16;
	desired.channels = 2;
	desired.samples = 512;// Vhodne pro hry
	desired.callback = AudioCallback;
	desired.userdata = NULL;

	// Otevre audio zarizeni
	if(SDL_OpenAudio(&desired, &obtained) == -1)
	{
		fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
		return 0;
	}

	// Loading audia a dekodovani (vse najednou)
	Sound_AudioInfo info;
	info.channels = obtained.channels;
	info.format = obtained.format;
	info.rate = obtained.freq;

	g_sample = Sound_NewSampleFromFile("bd.wav", &info, 512);
	if(g_sample == NULL)
	{
		fprintf(stderr, "Unable to load sound: %s\n", Sound_GetError());
		return 0;
	}

	// Dekoduje cely zvuk najednou
	Sound_DecodeAll(g_sample);
	if(g_sample->flags & SOUND_SAMPLEFLAG_ERROR)
	{
		fprintf(stderr, "Unable to decode sound: %s\n",
				Sound_GetError());
		return 0;
	}

	// Zacne prehravat
	SDL_PauseAudio(0);

	return 1;
}


/*
 * Deinicializacni funkce
 */

void Destroy()
{
	Sound_FreeSample(g_sample);	// Uvolni zvuk
	Sound_Quit();			// Ukonci SDL_sound
	SDL_CloseAudio();		// Zavre audio zarizeni
	SDL_Quit();			// Deinicializuje SDL
}


/*
 * Osetreni udalosti
 */

int ProcessEvent()
{
	SDL_Event event;

	while(SDL_WaitEvent(&event))
	{
		switch(event.type)
		{
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return 0;
				break;

			default:
				break;
			}
			break;

		case SDL_QUIT:
			return 0;
			break;

		default:
			break;
		}
	}

	return 1;
}


/*
 * Vstup do programu
 */

int main(int argc, char *argv[])
{
	printf("\nPress ESC key to quit.\n");

	// Inicializace
	if(!Init())
	{
		Destroy();
		return 1;
	}

	// Hlavni smycka programu
	int done = 0;
	while(!done)
	{
		done = !ProcessEvent();
	}

	// Deinicializace a konec
	Destroy();
	return 0;
}

