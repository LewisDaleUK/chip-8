#include "toolbox.h"
#include <SDL2/SDL.h>

#ifndef SCREEN_H
    #define SCREEN_H

    #define SCREEN_X 64
    #define SCREEN_Y 32
    #define SCALE 16

    #define COL_BLACK 0,0,0
    #define COL_WHITE 255,255,255

    typedef u8 Display[SCREEN_X][SCREEN_Y];

    extern Display DISPLAY;

    void clear_display(SDL_Renderer *renderer);
    void draw_display(SDL_Renderer *renderer, Display display);

#endif