#pragma once

#include <iostream>
#include <stdexcept>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "chip8.hpp"

class Window {

public:
    Window(int height);
    void draw_screen(uint32_t* pixels, int num_pixels);
    void quit();

private:
    int m_width;
    int m_height;
    SDL_Window* m_sdl_window;
    SDL_Renderer* m_sdl_renderer;
    SDL_Texture* m_sdl_texture;
};
