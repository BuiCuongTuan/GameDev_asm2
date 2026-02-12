#pragma once
#include <SDL3/SDL.h>

struct Platform {
    float x, y;
    float width, height;

    Platform(float startX, float startY, float w, float h);

    SDL_FRect getRect() const;
};