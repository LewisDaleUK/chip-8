#include "toolbox.h"
#include "screen.h"

#ifndef CHIP8_H
#define CHIP8_H

#define CHIP8_STACK_SIZE 16
#define TIMER_INITIAL_VALUE 60
#define V_REG_SIZE 16
#define KEYBOARD_SIZE 16

#define PROGRAM_START_ADDR 0x200
#define MAX_PROGRAM_LEN 3583

#define NO_KEY 0x10

typedef struct Chip8_S {
    u8 display[SCREEN_Y][SCREEN_X];
    u8 memory[4096];

    u16 pc;
    u16 instruction;
    u16 i;
    u16 stack[CHIP8_STACK_SIZE];

    u8 v_reg[V_REG_SIZE];
    u8 stack_reg;

    u8 delay_timer;
    u8 sound_timer;

    u8 keyboard[KEYBOARD_SIZE];
    u8 key_pressed;

    u8 updated_flag;
    u8 is_running;
} Chip8;

Chip8 chip8_init();

void load_rom(Chip8 *cpu, char *fn);
void update_timers(Chip8 *cpu);

void execute(Chip8 *cpu);
void fetch(Chip8 *cpu);
void decode(Chip8 *cpu);

#endif