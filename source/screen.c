#include "screen.h"

u8 DISPLAY[SCREEN_X][SCREEN_Y] = {0};


void clear_display(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void draw_display(SDL_Renderer *renderer, Display display) {
    int i, j;

    for (i = 0; i < SCREEN_X; i++) {
        for (j = 0; j < SCREEN_Y; j++) {
            u8 pt = display[i][j];

            if (pt == 1) {
                SDL_SetRenderDrawColor(renderer, COL_BLACK, 0);
            } else {
                SDL_SetRenderDrawColor(renderer, COL_WHITE, 0);
            }

            SDL_Rect draw_rect = {
                .x = i * SCALE,
                .y = j * SCALE,
                .w = SCALE,
                .h = SCALE
            };

            if (SDL_RenderFillRect(renderer, &draw_rect) != 0) {
                printf("Failed to draw the rect\n");
            }
        }
    }
}