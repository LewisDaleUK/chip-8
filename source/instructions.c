#include "toolbox.h"
#include "chip8.h"
#include "instructions.h"

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

void skip_x_nn_eq(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 nn = cpu->instruction & 0x00FF;

    if (cpu->v_reg[x] == nn) {
        cpu->pc += 2;
    }
}

void skip_x_nn_neq(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 nn = cpu->instruction & 0x00FF;

    if (cpu->v_reg[x] != nn) {
        cpu->pc += 2;
    }
}

void skip_x_y_eq(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 y = (cpu->instruction & 0x00F0) >> 4;

    if (cpu->v_reg[x] == cpu->v_reg[y]) {
        cpu->pc += 2;
    }
}

void skip_x_y_neq(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 y = (cpu->instruction & 0x00F0) >> 4;

    if (cpu->v_reg[x] != cpu->v_reg[y]) {
        cpu->pc += 2;
    }
}

void set_vx_vy(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 y = (cpu->instruction & 0x00F0) >> 4;

    cpu->v_reg[x] = cpu->v_reg[y];
}

void set_vx_or_vy(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 y = (cpu->instruction & 0x00F0) >> 4;

    cpu->v_reg[x] |= cpu->v_reg[y];
}

void set_vx_and_vy(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 y = (cpu->instruction & 0x00F0) >> 4;

    cpu->v_reg[x] &= cpu->v_reg[y];
}

void set_vx_xor_vy(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 y = (cpu->instruction & 0x00F0) >> 4;

    cpu->v_reg[x] ^= cpu->v_reg[y];
}

void set_vx_add_vy(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 y = (cpu->instruction & 0x00F0) >> 4;

    cpu->v_reg[x] += cpu->v_reg[y];
}

void sub_vx_vy(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 y = (cpu->instruction & 0x00F0) >> 4;

    cpu->v_reg[0xF] = TRUE;

    if (cpu->v_reg[x] <= cpu->v_reg[y]) {
        cpu->v_reg[0xF] = FALSE;
    }

    cpu->v_reg[x] -= cpu->v_reg[y];
}

void sub_vy_vx(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 y = (cpu->instruction & 0x00F0) >> 4;

    cpu->v_reg[0xF] = TRUE;

    if (cpu->v_reg[y] <= cpu->v_reg[x]) {
        cpu->v_reg[0xF] = FALSE;
    }

    cpu->v_reg[y] -= cpu->v_reg[x];
}

void shift_right(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 y = (cpu->instruction & 0x00F0) >> 4;
    
    cpu->v_reg[x] = cpu->v_reg[y];

    u8 vx = cpu->v_reg[x];
    cpu->v_reg[x] = vx >> 1;

    printf("vx: 0x%x\n", vx);
    printf("vx >> 1: 0x%x\n", vx >> 1);
    printf("vx & 0x01: 0x%x\n", vx & 0x01);

    printf("v_reg[x]: 0x%x\n", cpu->v_reg[x]);

    cpu->v_reg[0xF] = TRUE;
    if (!((vx & 0x01))) {
        cpu->v_reg[0xF] = 0;
    }
}

void shift_left(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 y = (cpu->instruction & 0x00F0) >> 4;
    
    cpu->v_reg[x] = cpu->v_reg[y];

    u8 vx = cpu->v_reg[x];
    cpu->v_reg[x] = vx << 1;

    cpu->v_reg[0xF] = TRUE;
    if (!((vx & 0x10) >> 8)) {
        cpu->v_reg[0xF] = 0;
    }
}

void jump_offset(Chip8 *cpu) {
    u8 offset = cpu->v_reg[0x0];
    u16 address = cpu->instruction & 0x0FFF;

    cpu->pc = address + offset;
}

void rand_vx_nn(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u16 nn = cpu->instruction & 0x00FF;

    u16 rint = rand();
    cpu->v_reg[x] = rint & nn;
}

void skip_if_key(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 key = cpu->v_reg[x];

    if (cpu->key_pressed == key) {
        cpu->pc += 2;
    }
}

void skip_if_not_key(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    u8 key = cpu->v_reg[x];

    if(cpu->key_pressed != key) {
        cpu->pc += 2;
    }
}

void set_vx_delay(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    cpu->v_reg[x] = cpu->delay_timer;
}

void set_delay_vx(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    cpu->delay_timer = cpu->v_reg[x];
}

void set_sound_vx(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    cpu->sound_timer = cpu->v_reg[x]; 
}

void add_to_index(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;
    cpu->i += cpu->v_reg[x];
}

void get_key(Chip8 *cpu) {
    if (cpu->key_pressed != NO_KEY) {
        u8 x = (cpu->instruction & 0x0F00) >> 8;
        cpu->v_reg[x] = cpu->key_pressed;
    } else {
        cpu->pc -= 2;
    }
}

void font_char(Chip8 *cpu) {
    u8 x = X_VAL;
    u8 vx = cpu->v_reg[x];

    // Multiply vx by 5 because the font sprites are 4*5 bytes
    cpu->i = vx * 0x5;
}

void binary_decimal_conversion(Chip8 *cpu) {
    u8 x = X_VAL;
    u8 vx = cpu->v_reg[x];

    cpu->memory[cpu->i] = vx / 100;
    cpu->memory[cpu->i + 1] = (vx / 10) % 10;
    cpu->memory[cpu->i + 2] = vx % 10;
}

void store_memory(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;

    for(int i = 0x0; i <= x; i++) {
        cpu->memory[cpu->i + i] = cpu->v_reg[i];
    }
}

void load_memory(Chip8 *cpu) {
    u8 x = (cpu->instruction & 0x0F00) >> 8;

    for(int i = 0x0; i <= x; i++) {
        cpu->v_reg[i] = cpu->memory[cpu->i + i];
    }
}
