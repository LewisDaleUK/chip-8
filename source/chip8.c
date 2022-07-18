#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "toolbox.h"
#include "chip8.h"
#include "font.h"
#include "screen.h"
#include "instructions.h"

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
        .pc = PROGRAM_START_ADDR,
        .key_pressed = NO_KEY
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


void decode(Chip8 *cpu) {
    if (!cpu->instruction) {
        exit(EXIT_FAILURE);
    }

    printf("Instruction: 0x%x\n", cpu->instruction);
    
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
                default:
                    printf("Unrecognised opcode: 0x%x\n", cpu->instruction);
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

        case 0x3000:
            skip_x_nn_eq(cpu);
            break;

        case 0x4000:
            skip_x_nn_neq(cpu);
            break;

        case 0x5000:
            skip_x_y_eq(cpu);
            break;

        case 0x6000:
            set_vx_nn(cpu);
            break;

        case 0x7000:
            add_vx_nn(cpu);
            break;

        case 0x8000:
            switch (cpu->instruction & 0x000F) {
                case 0x0000:
                    set_vx_vy(cpu);
                    break;
                case 0x0001:
                    set_vx_or_vy(cpu);
                    break;
                case 0x0002:
                    set_vx_and_vy(cpu);
                    break;
                case 0x0003:
                    set_vx_xor_vy(cpu);
                    break;
                case 0x0004:
                    set_vx_add_vy(cpu);
                    break;
                case 0x0005:
                    sub_vx_vy(cpu);
                    break;
                case 0x0006:
                    shift_right(cpu);
                    break;
                case 0x0007:
                    sub_vy_vx(cpu);
                    break;
                case 0x000E:
                    shift_left(cpu);
                    break;
                default:
                    printf("Unrecognised opcode: 0x%x\n", cpu->instruction);
            }
            break;
        
        case 0x9000:
            skip_x_y_neq(cpu);
            break;

        case 0xA000:
            set_index_register(cpu);
            break;

        case 0xB000:
            jump_offset(cpu);
            break;

        case 0xC000:
            rand_vx_nn(cpu);
            break;

        case 0xD000:
            draw(cpu);
            break;

        case 0xE000:
            switch (cpu->instruction & 0x00FF) {
                case 0x009E:
                    skip_if_key(cpu);
                    break;
                case 0x00A1:
                    skip_if_not_key(cpu);
                    break;
                default:
                    printf("Unrecognised opcode: 0x%x\n", cpu->instruction);
            }
            break;

        case 0xF000:
            switch (cpu->instruction & 0x00FF) {
                case 0x0007:
                    // timers
                    set_vx_delay(cpu);
                    break;
                case 0x0015:
                    // timers
                    set_delay_vx(cpu);
                    break;
                case 0x0018:
                    set_sound_vx(cpu);
                    // timers
                    break;
                case 0x001E:
                    add_to_index(cpu);
                    break;
                case 0x000A:
                    // get key
                    get_key(cpu);
                    break;
                case 0x0029:
                    // Font character
                    font_char(cpu);
                    break;
                case 0x0033:
                    // Binary coded decimal conversion
                    binary_decimal_conversion(cpu);
                    break;
                case 0x0055:
                    // Store memory
                    printf("Storing memory\n");
                    store_memory(cpu);
                    break;
                case 0x0065:
                    // Load memory
                    load_memory(cpu);
                    break;
                default:
                    printf("Unrecognised opcode: 0x%x\n", cpu->instruction);
            }
            break;
        default:
            printf("Unrecognised opcode: 0x%x\n", cpu->instruction);
    }
}