#include "../../include/core/Window.h"

#ifdef _WIN32
#include <windows.h>
#endif

Window::Window() : window(nullptr), renderer(nullptr) {}

Window::~Window() {
    clean();
}

bool Window::init(const char* title, int width, int height) {
    // Ẩn cửa sổ console trên Windows
#ifdef _WIN32
    HWND consoleWindow = GetConsoleWindow();
    ShowWindow(consoleWindow, SW_HIDE);
#endif

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    if (!SDL_CreateWindowAndRenderer(title, width, height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Could not create window/renderer: %s", SDL_GetError());
        return false;
    }
    
    SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    return true;
}

void Window::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Window::clear() {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); 
    SDL_RenderClear(renderer);
}

void Window::display() {
    SDL_RenderPresent(renderer);
}