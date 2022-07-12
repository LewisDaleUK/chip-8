#include <string.h>

#include "screen.h"

u32 PIX_BUFFER[SCREEN_X * SCREEN_Y] = { 0 };


void buffer_display(Display display) {
    memset(PIX_BUFFER, 0, sizeof(PIX_BUFFER));
    int x, y;

    for (x = 0; x < SCREEN_X; x++) {
        for (y = 0; y < SCREEN_Y; y++) {
            u8 pixel = display[y][x];
            PIX_BUFFER[(y * SCREEN_X) + x] = (0xFFFFFF00 * pixel) | 0x000000FF;
        }
    }
}

void clear_display(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, COL_BLACK, 0);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void draw_display(SDL_Renderer *renderer, SDL_Texture *texture, Display display) {
    buffer_display(display);
    SDL_UpdateTexture(texture, NULL, PIX_BUFFER, SCREEN_X * sizeof(u32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}