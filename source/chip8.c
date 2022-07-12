#include <string.h>

#include "toolbox.h"
#include "chip8.h"
#include "font.h"

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
        .stack = {0}
    };

    memcpy(&chip8.memory[FONT_IDX], &FONT, FONT_SIZE);

    return chip8;
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

void handle_instruction(Chip8 *cpu) {
    // Handle the possible instructions
}