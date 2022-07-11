#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "font.h"
#include "toolbox.h"
#include "screen.h"
#include "chip8.h"

#include "sprite.h"

void init_font(u8 memory[]) {
    memcpy(&memory[FONT_IDX], &FONT, FONT_SIZE);
}


int main() {
    Chip8 chip8 = chip8_init();

    init_font(chip8.memory);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Chip-8 Emulator",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_X * SCALE,
                                          SCREEN_Y * SCALE,
                                          0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTexture(renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, 
        SCREEN_X, 
        SCREEN_Y
    );
    
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
    SDL_Event e;

    clear_display(renderer);

    for (int i = 0; i < SPRITE_X; i++) {
        memcpy(&(chip8.display[i]), &SPRITE[i], SPRITE_Y * sizeof(u8));
    }

    int br_loop = 0;
    while(br_loop == 0) {
        while(SDL_PollEvent(&e) > 0 ) {
                // Do stuff
                draw_display(renderer, chip8.display);
                SDL_RenderPresent(renderer);
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