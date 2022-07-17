#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define X_VAL ((cpu->instruction & 0x0F00) >> 8)

void return_from_subroutine(Chip8 *cpu);
void set_vx_nn(Chip8 *cpu);
void add_vx_nn(Chip8 *cpu);
void set_index_register(Chip8 *cpu);
void draw(Chip8 *cpu);
void skip_x_nn_eq(Chip8 *cpu);
void skip_x_nn_neq(Chip8 *cpu);
void skip_x_y_eq(Chip8 *cpu);
void skip_x_y_neq(Chip8 *cpu);
void set_vx_vy(Chip8 *cpu);
void set_vx_or_vy(Chip8 *cpu);
void set_vx_and_vy(Chip8 *cpu);
void set_vx_xor_vy(Chip8 *cpu);
void set_vx_add_vy(Chip8 *cpu);
void sub_vx_vy(Chip8 *cpu);
void sub_vy_vx(Chip8 *cpu);
void shift_right(Chip8 *cpu);
void shift_left(Chip8 *cpu);
void jump_offset(Chip8 *cpu);
void rand_vx_nn(Chip8 *cpu);
void skip_if_key(Chip8 *cpu);
void skip_if_not_key(Chip8 *cpu);
void set_vx_delay(Chip8 *cpu);
void set_delay_vx(Chip8 *cpu);
void set_sound_vx(Chip8 *cpu);
void add_to_index(Chip8 *cpu);
void get_key(Chip8* cpu);
void font_char(Chip8 *cpu);
void binary_decimal_conversion(Chip8 *cpu);
void store_memory(Chip8 *cpu);
void load_memory(Chip8 *cpu);

#endif