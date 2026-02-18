#pragma once
#include <SDL3/SDL.h>
#include "../utils/Vector2.h"
#include "../utils/Constants.h"

struct Player {
    int id;
    Vector2 position;
    Vector2 velocity;
    float width, height;
    bool isGrounded;
    float hp;
    float maxHp;
    float mana;
    float maxMana;
    float shootCooldown;

    float currentChargeTime; 
    bool isCharging;

    float aimAngle;
    Uint8 r, g, b;

    Player(int playerId, float x, float y); 
    SDL_FRect getRect() const;
};