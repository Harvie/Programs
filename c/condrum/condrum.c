#include "bass.h"
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>

//#define DEBUG //Uncomment for debuging
#define MAX_CHANS 128 //Maximum lenght of line in loopfile (recomended 128 or 64)


// display error messages
void Error(const char *text) 
{
	printf("Error(%d): %s\n",BASS_ErrorGetCode(),text);
	BASS_Free();
	exit(0);
}

int main(int argc, char *argv)
{
    if (!BASS_Init(-1,44100,0,0,NULL)) {
       // couldn't initialize device
       printf("Error -1- Init");
       return(0);
       }
    
    char loopf[] = "loop.txt";
    int i = 0, t = 0, sch=sizeof(char), sleep = 500;  //t = total chanells
    HSAMPLE sample[MAX_CHANS];
    HCHANNEL channel[MAX_CHANS];
    char *line, bar[MAX_CHANS] = {'1', '0'};
    line = malloc(MAX_CHANS*sch);
    FILE *loop;
    
    loop = fopen(loopf, "r");
    
    printf("ConDrummer\nSimple Console Drumstation for Windows\nHarvie 2oo7\nFile: %s\n\n", loopf);
    
    while(1) {
             fgets(line, MAX_CHANS, loop);
             if(strstr(line, ";")) { 
                             //printf("%s", line+sch);
                             continue;
                             }
             if(strstr(line, "!")) { 
                             printf("%s", line+sch);
                             continue;
                             }
             if(strstr(line, ":")) {
                        line++;
                        memset( line+strlen(line)-1, 0, 1);
                        printf("Sample: %i = %s\n", i, line);
                        sprintf(line, "%s", line);
                        sample[i] =  BASS_SampleLoad(FALSE, line, 0, 0, 100, BASS_SAMPLE_MONO);
                        channel[i] = BASS_SampleGetChannel(sample[i], FALSE); // get a sample channel
                        i++;
                        t++;
                        line--;
                        continue;
                        }
                        
             if(strstr(line, "?")) {
                        sleep = atoi(line+sch);
                        #ifdef DEBUG
                        printf("Delay: %i\n", sleep);
                        #endif
                        continue;
                        }
             if(strstr(line, "@")) {
                        sleep = atoi(line+sch);
                        sleep = (60*1000)/(sleep*4);
                        #ifdef DEBUG
                        printf("Delay: %i\n", sleep);
                        #endif
                        continue;
                        }
                        
             if(strstr(line, "#")) {
                        break;
                        }
             }
     
    printf("Channels total: %i\nPlaying...\n\n", t);
     
    //DrumLOOP
    i = 0;
    while(1) {
             i = 0;
             while(1) {
                      fgets(line, MAX_CHANS, loop);
                      if(strstr(line, "-")) {
                                      sprintf(bar, "%s\x0", (line+sch));
                                      break;
                                      }
                        if(feof(loop)) {
                                        #ifdef DEBUG
                                               printf("rewind\n"); //Debug
                                        #endif
                                        rewind(loop);
                                        while(1) {
                                                 fgets(line, MAX_CHANS, loop);
                                                 if(strstr(line, "#")) { break; }
                                                 }
                                      }
                        }
    
             for(i = 0;i<t;i++) {
                   if (bar[i] == '1') {
                      BASS_ChannelPlay(channel[i], FALSE);
                   }
             }
        
             Sleep(sleep);
    
    }
    
	BASS_Free();
    return(0);
}

/*

TODO (unsorted):
     - better samples (Audacity or ER-0)
     - better (user friendly) loopfile syntax
     - BPM 2 sleep delay calculator
     - better icon
     - manual
     - sample patterns
     - Linux port (SDL???)
     - volume for each beat
     - recording to wav

*/



