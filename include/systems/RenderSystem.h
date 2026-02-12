#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <cmath>
#include "../entities/Player.h"
#include "../entities/Projectile.h"
#include "../entities/Platform.h"

class RenderSystem {
public:
    void render(SDL_Renderer* renderer, 
                const Player& player, 
                const std::vector<Projectile>& bullets, 
                const std::vector<Platform>& platforms, 
                SDL_Texture* texBullet,
                SDL_Texture* texP1,
                SDL_Texture* texP2) 
    {
        SDL_FRect rect = player.getRect();
        
        SDL_Texture* currentTex = (player.id == 1) ? texP1 : texP2;

        if (currentTex != nullptr) {
            SDL_FlipMode flip = SDL_FLIP_NONE;
            if (player.aimAngle > 90 && player.aimAngle < 270) {
                flip = SDL_FLIP_HORIZONTAL;
            }
            if (player.isCharging) {
                float ratio = player.currentChargeTime / MAX_CHARGE_TIME;
                Uint8 boost = (Uint8)(ratio * 150);
                SDL_SetTextureColorMod(currentTex, 255, 255 - boost, 255 - boost); 
            } else {
                SDL_SetTextureColorMod(currentTex, 255, 255, 255);
            }

            SDL_RenderTextureRotated(renderer, currentTex, NULL, &rect, 0, NULL, flip);
        } 
        else {
            if (player.isCharging) {
                float ratio = player.currentChargeTime / MAX_CHARGE_TIME;
                Uint8 whiteAdd = (Uint8)(ratio * 150);
                Uint8 r = (player.r + whiteAdd > 255) ? 255 : player.r + whiteAdd;
                Uint8 g = (player.g + whiteAdd > 255) ? 255 : player.g + whiteAdd;
                Uint8 b = (player.b + whiteAdd > 255) ? 255 : player.b + whiteAdd;
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, player.r, player.g, player.b, 255);
            }
            SDL_RenderFillRect(renderer, &rect);
        }

        float cx = player.position.x + player.width / 2;
        float cy = player.position.y + player.height / 2;
        float rad = player.aimAngle * (PI / 180.0f);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderLine(renderer, cx, cy, cx + std::cos(rad)*60, cy + std::sin(rad)*60);

        for (const auto& p : bullets) {
            if (!p.active) continue;
            SDL_FRect dst = {p.position.x, p.position.y, p.radius*2, p.radius*2};
            
            if (texBullet) {
                SDL_RenderTexture(renderer, texBullet, NULL, &dst);
            } else {
                if (p.ownerId == 1) SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                else                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                SDL_RenderFillRect(renderer, &dst);
            }
        }
    }
};