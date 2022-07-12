#include "toolbox.h"
#include <SDL2/SDL.h>

#ifndef SCREEN_H
    #define SCREEN_H

    #define SCREEN_X 64
    #define SCREEN_Y 32
    #define SCALE 16

    #define COL_BLACK 0,0,0
    #define COL_WHITE 255,255,255

    typedef u8 Display[SCREEN_Y][SCREEN_X];

    void buffer_display(Display display);
    void clear_display(SDL_Renderer *renderer);
    void draw_display(SDL_Renderer *renderer, SDL_Texture *texture, Display display);

#endif