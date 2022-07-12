#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "toolbox.h"
#include "screen.h"
#include "chip8.h"

#include "sprite.h"

int main() {
    Chip8 chip8 = chip8_init();

    int cycle_count = 0;

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

    for (int i = 0; i < SPRITE_Y; i++) {
        memcpy(&(chip8.display[i]), &SPRITE[i], SPRITE_X * sizeof(u8));
    }

    chip8.updated_flag = TRUE;

    while(chip8.is_running) {
        cycle_count++;

        if (cycle_count == TIMER_INTERVAL) {
            cycle_count = 0;

            update_timers(&chip8);
        }

        if (chip8.updated_flag) {
            chip8.updated_flag = FALSE;

            draw_display(renderer, texture, chip8.display);
            SDL_UpdateWindowSurface(window);
        }
                
        while(SDL_PollEvent(&e) > 0 ) {
            switch(e.type)
            {
                case SDL_QUIT:
                    chip8.is_running = FALSE;
                    break;
            }
        }
        usleep(SECOND_DELAY);   
    }
   
    return 0;
}