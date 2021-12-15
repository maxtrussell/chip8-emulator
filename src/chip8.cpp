#include "chip8.hpp"

#include <iostream>

unsigned char chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8() {
    debug = false;
}

Chip8::Chip8(bool is_debug) {
    debug = is_debug;
}

void Chip8::init() {
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    // Zero out attributes
    memset(gfx, 0, 2048);
    memset(memory, 0, 4096);
    memset(stack, 0, 16);
    memset(key, 0, 16);
    memset(V, 0, 16);

    // Copy font set into memory
    for (int i = 0; i < 80; i++) {
        memory[i] = chip8_fontset[i];
    }

    // Reset timers
    delay_timer = 0;
    sound_timer = 0;
}

bool Chip8::load_file(const char* path) {
    FILE* fp = fopen(path, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open rom\n");
        return false;
    }
    fseek(fp, 0, SEEK_END);
    long rom_size = ftell(fp);
    rewind(fp);

    // Copy ROM data in buffer
    unsigned char* rom_buffer = (unsigned char*) malloc(sizeof(char) * rom_size);
    fread(rom_buffer, sizeof(char), (size_t) rom_size, fp);

    load(rom_buffer, rom_size);

    fclose(fp);
    free(rom_buffer);
    return true;
}

void Chip8::load(const unsigned char* data, long data_size) {
    // Copy buffer into memory
    for (int i = 0; i < data_size; i++) {
        memory[i + 512] = data[i];  // rom data starts at 512
    }
}


void Chip8::set_key(int i, bool value) {
    key[i] = value;
}

void Chip8::emulate_cycle() {
    // Op code is two bytes
    opcode = memory[pc] << 8 | memory[pc+1];
    uint8_t x;
    uint8_t y;

    if (debug) {
        fprintf(stderr, "Op code: 0x%X\n", opcode);
    }

    // Switch on first 4 bits
    switch(opcode & 0xF000) {
    case(0x0000):
        switch (opcode & 0x00FF) {
        case(0x00E0):
            // 00E0: Clear the screen
            memset(gfx, 0, 2048);
            pc += 2;
            drawFlag = true;
            break;
        case(0x00EE):
            // 00EE: Return from a subroutine
            sp--;
            pc = stack[sp];
            pc += 2;
            break;
        default:
            fprintf(stderr, "Unknown opcode [0x000*]: 0x%X\n", opcode);
            exit(1);
        }
        break;
    case(0x1000):
        // 1NNN: jumps to addresss NNN
        pc = opcode & 0x0FFF;
        break;
    case(0x2000):
        // 2NNN: Call subroutine at NNN
        // Store current pc address on stack, then jump pc to NNN
        // Note: do not increment pc!
        stack[sp++] = pc;
        pc = opcode & 0x0FFF;
        break;
    case(0x3000):
        // 3XNN: Skips next instr if V[x] == NN
        x = (opcode & 0x0F00) >> 8;
        if (V[x] == (opcode & 0x00FF))
            pc += 2;
        pc += 2;
        break;
    case(0x4000):
        // 4XNN: Skips next instr if V[x] != NN
        x = (opcode & 0x0F00) >> 8;
        if (V[x] != (opcode & 0x00FF))
            pc += 2;
        pc += 2;
        break;
    case(0x5000):
        // 5XY0: Skips next instr if V[x] == V[y]
        x = (opcode & 0x0F00) >> 8;
        y = (opcode & 0x00F0) >> 4;
        if (V[x] == V[y])
            pc += 2;
        pc += 2;
        break;
    case(0x6000):
        // 6XNN: Sets V[x] to NN
        x = (opcode & 0x0F00) >> 8;
        V[x] = (opcode & 0x00FF);
        pc += 2;
        break;
    case(0x7000):
        // 7XNN: Adds NN into V[x] (carry flag is not changed)
        x = (opcode & 0x0F00) >> 8;
        V[x] += (opcode & 0x00FF);
        pc += 2;
        break;
    case(0x8000):
        switch(opcode & 0x000F) {
        case (0x0000):
            // 8XY0: sets V[x] to V[y]
            x = (opcode & 0x0F00) >> 8;
            y = (opcode & 0x00F0) >> 4;
            V[x] = V[y];
            pc += 2;
            break;
        case (0x0001):
            // 8XY1: Sets V[x] |= V[y]
            x = (opcode & 0x0F00) >> 8;
            y = (opcode & 0x00F0) >> 4;
            V[x] |= V[y];
            pc += 2;
            break;
        case (0x0002):
            // 8XY2: Sets V[x] &= V[y]
            x = (opcode & 0x0F00) >> 8;
            y = (opcode & 0x00F0) >> 4;
            V[x] &= V[y];
            pc += 2;
            break;
        case (0x0003):
            // 8XY3: Sets V[x] ^= V[y]
            x = (opcode & 0x0F00) >> 8;
            y = (opcode & 0x00F0) >> 4;
            V[x] ^= V[y];
            pc += 2;
            break;
        case (0x0004):
            // 8XY4: add V[Y] into V[X], set carry to V[0xF] if sum > 0xFF
            x = (opcode & 0x0F00) >> 8;
            y = (opcode & 0x00F0) >> 4;
            if (V[x] > (0xFF - V[y]))
                V[0xF] = 1;
            else
                V[0xF] = 0;
            V[x] += V[y];
            pc += 2;
            break;
        case (0x0005):
            // 8XY5: VX -= VY, VF is set to 0 if there's a borrow else 1
            x = (opcode & 0x0F00) >> 8;
            y = (opcode & 0x00F0) >> 4;
            V[0xF] = V[y] > V[x] ? 0 : 1;
            V[x] -= V[y];
            pc += 2;
            break;
        case (0x0006):
            // 8XY6: Store least sig bit of VX in VF and shifts VX to the right by 1
            x = (opcode & 0x0F00) >> 8;
            V[0xF] = (V[x] & 0x0001);
            V[x] >>= 1;
            pc += 2;
            break;
        case (0x0007):
            // 8XY7: Sets VX to VY - VX. VF is set to 0 when there's a borrow else 1
            x = (opcode & 0x0F00) >> 8;
            y = (opcode & 0x00F0) >> 4;
            V[0xF] = V[x] > V[y] ? 0 : 1;
            V[x] = V[y] - V[x];
            pc += 2;
            break;
        case (0x000E):
            // 8XYE: Store most sig bit of VX in VF and shifts VX to the left by 1
            x = (opcode & 0x0F00) >> 8;
            V[0xF] = (V[x] >> 7);
            V[x] <<= 1;
            pc += 2;
            break;
        default:
            fprintf(stderr, "Unknown opcode [0x800*]: 0x%X\n", opcode);
            exit(1);
        }
        break;
    case(0x9000):
        // 9XY0: skips next instr if VX != XY
        x = (opcode & 0x0F00) >> 8;
        y = (opcode & 0x00F0) >> 4;
        if (V[x] != V[y])
            pc += 2;
        pc += 2;
        break;
    case(0xA000):
        // ANNN: Sets I to address NNN
        // I = opcode && 0x0FFF;
        I = opcode & 0x0FFF;
        pc += 2;
        break;
    case(0xB000):
        // BNNN: jumps to address NNN + V0
        pc = (opcode & 0x0FFF) + V[0];
        break;
    case(0xC000):
        // CXNN: VX = rand() & NN, 0 <= rand() <= 255
        // for random number generation see:
        //   https://stackoverflow.com/a/12657984
        x = (opcode & 0x0F00) >> 8;
        V[x] = (opcode & 0x00FF) & (rand() % (0xFF + 1));
        pc += 2;
        break;
    case(0xD000): {
        // DXYN: Draw sprite
        x = V[(opcode & 0x0F00) >> 8];
        y = V[(opcode & 0x00F0) >> 4];
        uint8_t height = opcode & 0x000F;
        uint8_t pixel;
        uint16_t idx;
        
        V[0xF] = 0;
        for (int dy = 0; dy < height; dy++) {
            pixel = memory[I + dy];
            for (int dx = 0; dx < 8; dx++) {
                if ((pixel & (0x80 >> dx)) != 0) {
                    // Sprite has a 1 in this position
                    idx = x + dx + ((y + dy) * 64);
                    if (gfx[idx] == 1)
                        V[0xF] = 1;
                    gfx[idx] ^= 1;
                }
            }
        }
        drawFlag = true;
        pc += 2;
    }
        break;
    case(0xE000):
        switch (opcode & 0x00FF) {
        case(0x009E):
            // EX9E: Skips the next instruction if key[V[X]] is pressed
            x = (opcode & 0x0F00) >> 8;
            if (key[V[x]] != 0)
                pc += 2;
            pc +=2;
            break;
        case(0x00A1):
            // EXA1: Skips the next instruction if key[V[X]] is not pressed
            x = (opcode & 0x0F00) >> 8;
            if (key[V[x]] == 0)
                pc += 2;
            pc +=2;
            break;
        default:
            fprintf(stderr, "Unknown opcode [0xE0**]: 0x%X\n", opcode);
            exit(1);
        }
        break;
    case(0xF000):
        switch (opcode & 0x0FF) {
        case(0x0007):
            // FX07: sets VX to delay timer
            x = (opcode & 0x0F00) >> 8;
            V[x] = delay_timer;
            pc += 2;
            break;
        case(0x000A): {
            // FX0A: block until keypress, store keypress in VX
            bool key_pressed = false;
            x = (opcode & 0x0F00) >> 8;
            for (int i = 0; i < 16; i++) {
                if (key[i] != 0) {
                    V[x] = i;
                    key_pressed = true;
                }
            }
            if (!key_pressed)
                return;
            pc += 2;
        }
            break;
        case(0x0015):
            // FX15: sets delay timer to VX
            x = (opcode & 0x0F00) >> 8;
            delay_timer = V[x];
            pc += 2;
            break;
        case(0x0018):
            // FX18: sets sound timer to VX
            x = (opcode & 0x0F00) >> 8;
            sound_timer = V[x];
            pc += 2;
            break;
        case(0x001E):
            // FX1E: I += VX; VF is not affected
            // NOTE: I is supposed to be 12 bits
            x = (opcode & 0x0F00) >> 8;
            I += V[x];
            pc += 2;
            break;
        case(0x0029):
            // FX29: Sets I to the location of the sprite for the character in VX.
            x = (opcode & 0x0F00) >> 8;
            // The font goes 0 to F, each character is made up of 5 elements
            I = V[x] * 5;
            pc += 2;
            break;
        case(0x0033):
            // FX33: store binary-coded decimal representation of V[X] at the addresses I, I+1, and I+2
            // e.g. for V[X] == 150: V[i] = 1; V[i+1] = 5; v[i+2] = 0
            x = (opcode & 0x0F00) >> 8;
            memory[I] = V[x] / 100;
            memory[I+1] = (V[x] % 100) / 10;
            memory[I+2] = V[x] % 10;
            pc += 2;
            break;
        case(0x0055):
            // FX55: stores from V0 to VX into memory, starting at address I. I is not modified.
            x = (opcode & 0x0F00) >> 8;
            for (int i = 0; i <= x; i++) {
                memory[I+i] = V[i];
            }
            pc += 2;
            break;
        case(0x0065):
            // FX65: Fills from V0 to VX from memory, starting at address I. I is not modified.
            x = (opcode & 0x0F00) >> 8;
            for (int i = 0; i <= x; i++) {
                V[i] = memory[I+i];
            }
            pc += 2;
            break;
        default:
            fprintf(stderr, "Unknown opcode [0xF0**]: 0x%X\n", opcode);
            exit(1);
        }
        break;
    default:
        fprintf(stderr, "Unknown opcode [0x*000]: 0x%X\n", opcode);
        exit(1);
    }

    if (delay_timer > 0)
        delay_timer--;

    if (sound_timer > 0) {
        sound_timer--;
        if (sound_timer == 0) {
            // TODO: BEEP!
        }
    }
}
