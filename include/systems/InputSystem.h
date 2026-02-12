#pragma once
#include <SDL3/SDL.h>
#include "../entities/Player.h"
#include "../utils/Constants.h"

class InputSystem {
public:
    void update(Player& player, float deltaTime, const bool* keys) {
        player.velocity.x = 0;
        if (player.id == 1) {
            if (keys[SDL_SCANCODE_A]) player.velocity.x = -MOVE_SPEED;
            if (keys[SDL_SCANCODE_D]) player.velocity.x = MOVE_SPEED;
            if (keys[SDL_SCANCODE_W] && player.isGrounded) { 
                player.velocity.y = JUMP_FORCE; player.isGrounded = false; 
            }
        } else {
            if (keys[SDL_SCANCODE_LEFT]) player.velocity.x = -MOVE_SPEED;
            if (keys[SDL_SCANCODE_RIGHT]) player.velocity.x = MOVE_SPEED;
            if (keys[SDL_SCANCODE_UP] && player.isGrounded) { 
                player.velocity.y = JUMP_FORCE; player.isGrounded = false; 
            }
        }

        SDL_Scancode shootKey = (player.id == 1) ? SDL_SCANCODE_SPACE : SDL_SCANCODE_RETURN;

        if (keys[shootKey]) {
            if (player.mana >= MANA_COST) {
                player.isCharging = true;
                player.currentChargeTime += deltaTime;
                if (player.currentChargeTime > MAX_CHARGE_TIME) player.currentChargeTime = MAX_CHARGE_TIME;
            }
        } else {
            player.isCharging = false;
        }
    }
};