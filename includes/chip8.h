#include "toolbox.h"
#include "screen.h"

#ifndef CHIP8_H
#define CHIP8_H

#define CHIP8_STACK_SIZE 16

typedef struct Chip8_S {
    u8 display[SCREEN_Y][SCREEN_X];
    u8 memory[4096];
    u16 stack[CHIP8_STACK_SIZE];

    u8 delay_timer;
    u8 sound_timer;

    
} Chip8;

Chip8 chip8_init();

#endif