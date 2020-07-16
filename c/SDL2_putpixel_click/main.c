//gcc -Wextra -pedantic-errors -o main.out main.c -lSDL2

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#define WINDOW_WIDTH 600

int main(void) {
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    int i;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, SDL_WINDOW_RESIZABLE, &window, &renderer);
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    //SDL_RenderSetScale(renderer, 2.0, 2.0);
    SDL_SetWindowTitle(window, "Simple SDL paint");


    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (i = 0; i < WINDOW_WIDTH; ++i)
      SDL_RenderDrawPoint(renderer, i, i);

    SDL_RenderPresent(renderer);

    int running = 1;
    int mouseX, mouseY;
    Uint32 fullscreen_status = 0;

    while(running) {
	event.type = 0;
	if(SDL_PollEvent(&event)) {
	   switch(event.type){
	     case SDL_WINDOWEVENT:
		break;
             case SDL_QUIT:
                running = 0;
                break;
             case SDL_KEYDOWN:
                //case SDL_KEYUP:
                printf("Key: %d\t%d\t%d\n", event.key.keysym.scancode, event.key.keysym.sym, event.key.keysym.mod );

                if(event.key.keysym.scancode == SDL_SCANCODE_F && event.key.keysym.mod == KMOD_LCTRL) {
                     fullscreen_status = fullscreen_status ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP;
                     SDL_SetWindowFullscreen(window, fullscreen_status);
                }

                if(event.key.keysym.scancode == SDL_SCANCODE_Q && event.key.keysym.mod == KMOD_LCTRL) {
                     running = 0;
                }

                break;
             case SDL_MOUSEBUTTONDOWN:
                //do whatever you want to do after a mouse button was pressed,
                // e.g.:
		//if(event.button.button == SDL_BUTTON_LEFT) {
		  while(event.type != SDL_MOUSEBUTTONUP) {
		        SDL_GetMouseState(&mouseX, &mouseY);
	        	SDL_RenderDrawPoint(renderer, mouseX, mouseY);
	        	SDL_RenderDrawPoint(renderer, mouseX, mouseY+1);
	        	SDL_RenderDrawPoint(renderer, mouseX+1, mouseY);
	        	SDL_RenderDrawPoint(renderer, mouseX+1, mouseY+1);
		        SDL_RenderPresent(renderer);
			SDL_PollEvent(&event);
		  }
		  printf("Click: %d\t%d\t%d\n", mouseX, mouseY, event.button.button);
		//}
                break;
           }
        }
	usleep(10000);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
