#include "window.hpp"

Window::Window(int height) {
    int width = height * 2;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::string error_msg = std::string("Error: SDL initialization failed: ") + SDL_GetError();
        throw std::runtime_error(error_msg);
    }

    m_sdl_window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );
    if (m_sdl_window == NULL) {
        std::string error_msg = std::string("Error: Window creation failed: ") + SDL_GetError();
        throw std::runtime_error(error_msg);
    }

    m_sdl_renderer = SDL_CreateRenderer(m_sdl_window, -1, 0);
    SDL_RenderSetLogicalSize(m_sdl_renderer, width, height);

    m_sdl_texture = SDL_CreateTexture(
        m_sdl_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        64,
        32
    );
}

void Window::quit() {
    SDL_DestroyWindow(m_sdl_window);
    SDL_Quit();
}

void Window::draw_screen(uint32_t* pixels, int num_pixels) {
    SDL_UpdateTexture(m_sdl_texture, NULL, pixels, 64 * sizeof(Uint32));
    SDL_RenderClear(m_sdl_renderer);
    SDL_RenderCopy(m_sdl_renderer, m_sdl_texture, NULL, NULL);
    SDL_RenderPresent(m_sdl_renderer);
}
