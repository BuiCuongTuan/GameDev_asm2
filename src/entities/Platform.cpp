#include "../../include/entities/Platform.h"

Platform::Platform(float startX, float startY, float w, float h) 
    : x(startX), y(startY), width(w), height(h) 
{
}
SDL_FRect Platform::getRect() const {
    return { x, y, width, height };
}