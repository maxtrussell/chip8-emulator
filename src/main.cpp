#include <chrono>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <cassert>

#include <SDL2/SDL.h>

#include "main.hpp"
#include "tests.hpp"
#include "window.hpp"

using namespace std;

#define NUM_PIXELS 2048

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "Usage: ./chip8 [path to ROM]\n");
        return 1;
    }

    bool debug = getenv("CHIP8_DEBUG");
    if (!strcmp(*(argv + 1), "test")) {
        return test(debug);
    }

    // Chip-8 screen is 64x32
    Window window = Window(512);
    uint32_t pixels[NUM_PIXELS];
    fill(pixels, pixels + NUM_PIXELS, 0);

    Chip8 chip8 = Chip8(debug);
    chip8.init();
    chip8.load_file(*(argv + 1));

    // Emulation loop
    while(true) {
        chip8.emulate_cycle();
        if (poll(&chip8) < 0) {
            window.quit();
            break;
        }

        if(chip8.drawFlag) {
            // Update pixels from chip8.gfx
            for (int i = 0; i < 2048; i++) {
                pixels[i] = (0x00FFFFFF * chip8.gfx[i]) | 0xFF000000;
            }
            window.draw_screen(pixels, NUM_PIXELS);
            chip8.drawFlag = false;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(1200));
    }
    return 0;
}

int poll(Chip8* chip8) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return -1;
        }

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                return -1;
            }

            for (int i = 0; i < 16; i++) {
                if (e.key.keysym.sym == chip8->keymap[i]) {
                    chip8->set_key(i, true);
                }
            }
        }

        if (e.type == SDL_KEYUP) {
            for (int i = 0; i < 16; i++) {
                if (e.key.keysym.sym == chip8->keymap[i]) {
                    chip8->set_key(i, false);
                }
            }
        }
    }
    return 0;
}

int test(bool debug) {
    Tests tests = Tests(debug);
    tests.run_tests();
    printf("Tests complete!\n");
    return 0;
}
