#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "toolbox.h"
#include "screen.h"
#include "keymap.h"
#include "chip8.h"

u8 map_key(SDL_KeyCode keycode) {
    for (int i = 0; i < KEYBOARD_SIZE; i++) {
        if (KEYMAP[i] == keycode) {
            return i;
        }
    }

    return 0;
}

void handle_events(Chip8 *chip8) {
    SDL_Event e;
    u8 key;
    
    while( SDL_PollEvent(&e) ) {
        switch(e.type) {
            case SDL_KEYDOWN:
                key = map_key(e.key.keysym.sym);

                if (key) {
                    chip8->key_pressed = &chip8->keyboard[key];
                }

                break;
            case SDL_KEYUP:
                chip8->key_pressed = NULL;
                break;
            case SDL_QUIT:
                chip8->is_running = FALSE;
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    Chip8 chip8 = chip8_init();

    load_rom(&chip8, argv[1]);

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

    while(chip8.is_running) {
        cycle_count++;

        execute(&chip8);

        if (cycle_count == TIMER_INTERVAL) {
            cycle_count = 0;

            update_timers(&chip8);
        }

        if (chip8.updated_flag) {
            chip8.updated_flag = FALSE;

            draw_display(renderer, texture, chip8.display);
            SDL_UpdateWindowSurface(window);
        }
                
        handle_events(&chip8);
        usleep(SECOND_DELAY);   
    }
   
    return 0;
}