#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "font.h"
#include "toolbox.h"

void init_font() {
    memcpy(&MEMORY[FONT_IDX], &FONT, FONT_SIZE);
}

int main() {
    printf("Hello world\n");

    init_font();

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          680, 480,
                                          0);
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    SDL_Event e;
    int br_loop = 0;
    while(br_loop == 0) {
        while(SDL_PollEvent(&e) > 0 ) {
                // Do stuff
                SDL_UpdateWindowSurface(window);

                switch(e.type)
                {
                    case SDL_QUIT:
                        br_loop = 1;
                        break;
                }

            }
    }
   
    return 0;
}