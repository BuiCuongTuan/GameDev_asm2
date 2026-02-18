#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <cmath>
#include "../entities/Player.h"
#include "../entities/Projectile.h"
#include "../entities/Platform.h"
#include "../utils/Constants.h"

class RenderSystem {
public:
    void render(SDL_Renderer* renderer, 
                const Player& p1, 
                const Player& p2,
                const std::vector<Projectile>& bullets, 
                const std::vector<Platform>& platforms, 
                SDL_Texture* texBullet,
                SDL_Texture* texP1,
                SDL_Texture* texP2) 
    {
        renderPlayer(renderer, p1, texP1, texP2);
        renderPlayer(renderer, p2, texP1, texP2);

        for (const auto& p : bullets) {
            if (!p.active) continue;
            SDL_FRect dst = {p.position.x, p.position.y, p.radius*2, p.radius*2};
            if (texBullet) {
                SDL_RenderTexture(renderer, texBullet, NULL, &dst);
            } else {
                if (p.ownerId == 1) SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                else SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                SDL_RenderFillRect(renderer, &dst);
            }
        }

        renderUI(renderer, p1, p2);
    }

private:
    void renderPlayer(SDL_Renderer* renderer, const Player& player, SDL_Texture* texP1, SDL_Texture* texP2) {
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
        } else {
            SDL_SetRenderDrawColor(renderer, player.r, player.g, player.b, 255);
            SDL_RenderFillRect(renderer, &rect);
        }

        float cx = player.position.x + player.width / 2;
        float cy = player.position.y + player.height / 2;
        float rad = player.aimAngle * (PI / 180.0f);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 150);
        SDL_RenderLine(renderer, cx, cy, cx + std::cos(rad)*60, cy + std::sin(rad)*60);
    }

    void renderUI(SDL_Renderer* renderer, const Player& p1, const Player& p2) {
        float barW = 300.0f;
        float hpH = 25.0f;
        float manaH = 10.0f;
        float offset = 20.0f;

        // --- PLAYER 1 ---
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_FRect bg1 = { offset, offset, barW, hpH };
        SDL_RenderFillRect(renderer, &bg1);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        float p1HpW = (p1.hp / p1.maxHp) * barW;
        SDL_FRect hp1 = { offset, offset, p1HpW, hpH }; 
        SDL_RenderFillRect(renderer, &hp1);

        SDL_SetRenderDrawColor(renderer, 0, 150, 255, 255);
        float p1ManaW = (p1.mana / p1.maxMana) * barW;
        SDL_FRect mana1 = { offset, offset + hpH + 2, p1ManaW, manaH };
        SDL_RenderFillRect(renderer, &mana1);

        // --- PLAYER 2 ---
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_FRect bg2 = { SCREEN_WIDTH - barW - offset, offset, barW, hpH };
        SDL_RenderFillRect(renderer, &bg2);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        float p2HpW = (p2.hp / p2.maxHp) * barW;
        SDL_FRect hp2 = { SCREEN_WIDTH - offset - p2HpW, offset, p2HpW, hpH };
        SDL_RenderFillRect(renderer, &hp2);

        SDL_SetRenderDrawColor(renderer, 0, 150, 255, 255);
        float p2ManaW = (p2.mana / p2.maxMana) * barW;
        SDL_FRect mana2 = { SCREEN_WIDTH - offset - p2ManaW, offset + hpH + 2, p2ManaW, manaH };
        SDL_RenderFillRect(renderer, &mana2);
    }
};