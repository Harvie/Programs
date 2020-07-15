//gcc -Wextra -pedantic-errors -o main.out main.c -lSDL2

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
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (i = 0; i < WINDOW_WIDTH; ++i)
      SDL_RenderDrawPoint(renderer, i, i);

    SDL_RenderPresent(renderer);

    int running = 1;
    int mouseX, mouseY;
    while(running) {
	event.type = 0;
	if(SDL_PollEvent(&event)) {
	   switch(event.type){
             case SDL_QUIT:
                running = 0;
                break;
             case SDL_MOUSEBUTTONDOWN:
                //do whatever you want to do after a mouse button was pressed,
                // e.g.:
		//if(event.button.button == SDL_BUTTON_LEFT) {
		  SDL_GetMouseState(&mouseX, &mouseY);
		  printf("Click %d, %d, %d\n", mouseX, mouseY, event.button.button);
        	  SDL_RenderDrawPoint(renderer, mouseX, mouseY);
		  SDL_RenderPresent(renderer);
		//}
                break;
           }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
