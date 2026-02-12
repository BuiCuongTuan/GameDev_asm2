#pragma once
#include <SDL3/SDL.h>
#include <iostream>

class Window {
public:
    Window();
    ~Window();

    bool init(const char* title, int width, int height);
    void clean();
    void clear();
    void display();

    SDL_Renderer* getRenderer() const { return renderer; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};