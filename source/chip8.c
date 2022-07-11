#include "chip8.h"

Chip8 chip8_init() {
    Chip8 chip8 = {
        .display = {0}
    };
    return chip8;
}