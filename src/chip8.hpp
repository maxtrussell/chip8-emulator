#pragma once

#include <SDL2/SDL.h>

/*
  See: https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/

  Memory Map:
  0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
  0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
  0x200-0xFFF - Program ROM and work RAM
*/

class Chip8 {
public:
    Chip8();
    Chip8(bool);
    void init();
    bool load_file(const char*);
    void load(const unsigned char* data, long data_size);
    void emulate_cycle();
    void set_key(int, bool);

    uint16_t pc;           // program counter
    bool debug;
    bool drawFlag;
    uint8_t gfx[64*32];    // array of pixels, 1=white, 0=black
    uint8_t keymap[16] = {
        SDLK_x,
        SDLK_1,
        SDLK_2,
        SDLK_3,
        SDLK_q,
        SDLK_w,
        SDLK_e,
        SDLK_a,
        SDLK_s,
        SDLK_d,
        SDLK_z,
        SDLK_c,
        SDLK_4,
        SDLK_r,
        SDLK_f,
        SDLK_v,
    };

    uint16_t opcode;

    uint8_t memory[4096];  // 4KB system memory

    uint8_t V[16];         // V registers
    uint16_t I;            // I register

    uint8_t delay_timer;
    uint8_t sound_timer;

    uint16_t stack[16];
    uint16_t sp;           // stack pointer

    uint8_t key[16];       // hex based keypad
};
