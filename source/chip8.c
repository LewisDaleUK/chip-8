#include <string.h>
#include <stdio.h>

#include "toolbox.h"
#include "chip8.h"
#include "font.h"
#include "screen.h"

Chip8 chip8_init() {
    Chip8 chip8 = {
        .display = {0},
        .delay_timer = TIMER_INITIAL_VALUE,
        .sound_timer = TIMER_INITIAL_VALUE,
        .updated_flag = FALSE,
        .stack_reg = 0,
        .v_reg = {0},
        .memory = {0},
        .is_running = TRUE,
        .stack = {0},
        .pc = PROGRAM_START_ADDR
    };

    memcpy(&chip8.memory[FONT_IDX], &FONT, FONT_SIZE);

    return chip8;
}

void load_rom(Chip8 *cpu, char *fn) {
    FILE *file = fopen(fn, "r");

    if (file != NULL) {
        long rom_length;

        fseek(file, 0, SEEK_END);
        rom_length = ftell(file);
        rewind(file);

        printf("File is %ld items long\n", rom_length);
        fread(&cpu->memory[PROGRAM_START_ADDR], sizeof(u8), rom_length, file);
    }

    fclose(file);
}

void update_timers(Chip8 *cpu) {
    cpu->delay_timer--;
    cpu->sound_timer--;

    if (!cpu->delay_timer) {
        cpu->delay_timer = TIMER_INITIAL_VALUE;
    }

    if (!cpu->sound_timer) {
        cpu->sound_timer = TIMER_INITIAL_VALUE;
    }
}

void execute(Chip8 *cpu) {
    // Handle the possible instructions
    fetch(cpu);
    decode(cpu);
}

void fetch(Chip8 *cpu) {
    cpu->instruction = (u16) cpu->memory[cpu->pc] << 8 | cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
}

void return_from_subroutine(Chip8 *cpu) {
    u16 addr = cpu->stack[cpu->stack_reg];
    cpu->stack[cpu->stack_reg] = 0;
    cpu->stack_reg--;

    cpu->pc = addr;   
}

void set_vx_nn(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u16 nn = (cpu->instruction & 0x00FF);

    cpu->v_reg[x] = nn;
}

void add_vx_nn(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u16 nn = (cpu->instruction & 0x00FF);

    cpu->v_reg[x] += nn;
}

void set_index_register(Chip8 *cpu) {
    u16 nnn = cpu->instruction & 0x0FFF;
    cpu->i = nnn;
}

void draw(Chip8 *cpu) {
    u8 vx = (cpu->instruction & 0x0F00) >> 8;
    u8 vy = (cpu->instruction & 0x00F0) >> 4;
    u8 n = cpu->instruction & 0x000F;

    u8 x = cpu->v_reg[vx] & (SCREEN_X - 1);
    u8 y = cpu->v_reg[vy] & (SCREEN_Y - 1);
    cpu->v_reg[0xF] = FALSE;

    u8 sprite;

    int row, col;

    for (row = 0; row < n; row++) {
        sprite = cpu->memory[cpu->i + row];
        for (col = 0; col < 8; col++) {
            
            // Sprite is an 8-bit unsigned int
            // Mask out everything but the relevant bit depending on the column, and check if it is 1
            // e.g. if the sprite is 100010000
            // and we're at col 0
            // then (10001000 & (0x80 >> col)) == 1
            if ((sprite & (0x80 >> col)) != 0) {
                // If there is already a pixel at the location we're looking at
                // Set the collision register entry (v[15]) to 1
                if (cpu->display[y + row][x + col] == 1) {
                    cpu->v_reg[0xF] = TRUE;
                }
                // Use XOR to flip the pixel
                cpu->display[y + row][x + col] ^= 1;
            }
        }
    }

    cpu->updated_flag = TRUE;
}

void decode(Chip8 *cpu) {
    // Decode the fetched instruction

    if (!cpu->instruction) {
        exit(EXIT_FAILURE);
    }

    switch(cpu->instruction & 0xF000) {
        case 0x0000:
            switch(cpu->instruction & 0x00FF) {
                case 0x00E0:
                    // Clear the display
                    memset(cpu->display, 0, sizeof(cpu->display));
                    cpu->updated_flag = TRUE;
                    break;

                case 0x00EE:
                    return_from_subroutine(cpu);                 
                    break;
            }
            break;

        case 0x1000:
            cpu->pc = cpu->instruction & 0x0FFF;
            break;

        case 0x2000:
            cpu->stack_reg++;
            cpu->stack[cpu->stack_reg] = cpu->pc;
            cpu->pc = cpu->instruction & 0x0FFF;
            break;

        case 0x6000:
            set_vx_nn(cpu);
            break;

        case 0x7000:
            add_vx_nn(cpu);
            break;

        case 0xA000:
            set_index_register(cpu);
            break;

        case 0xD000:
            draw(cpu);
            break;
    }
}