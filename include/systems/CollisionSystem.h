#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "../entities/Player.h"
#include "../entities/Projectile.h"
#include "../entities/Platform.h"
#include "../utils/Constants.h"

class CollisionSystem {
public:
    void update(Player& player, std::vector<Projectile>& bullets, const std::vector<Platform>& platforms) {
        player.isGrounded = false;
        if (player.position.x < 0) player.position.x = 0;
        if (player.position.x > SCREEN_WIDTH - player.width) player.position.x = SCREEN_WIDTH - player.width;
        if (player.position.y > SCREEN_HEIGHT - player.height) {
            player.position.y = SCREEN_HEIGHT - player.height;
            player.velocity.y = 0;
            player.isGrounded = true;
        }
        for (const auto& plat : platforms) {
            bool collisionX = (player.position.x + player.width > plat.x) && (player.position.x < plat.x + plat.width);
            bool falling = player.velocity.y >= 0;
            bool feetHitting = (player.position.y + player.height >= plat.y) && (player.position.y + player.height <= plat.y + 20);
            if (collisionX && falling && feetHitting) {
                player.position.y = plat.y - player.height;
                player.velocity.y = 0;
                player.isGrounded = true;
            }
        }

        for (auto& p : bullets) {
            if (!p.active) continue;

            if (p.existTime < 0.05f) {
                continue; 
            }

            bool bounced = false;

            if (p.position.x <= 0) { 
                p.position.x = 0; p.velocity.x = -p.velocity.x; bounced = true; 
            }
            else if (p.position.x >= SCREEN_WIDTH - p.radius*2) { 
                p.position.x = SCREEN_WIDTH - p.radius*2; p.velocity.x = -p.velocity.x; bounced = true; 
            }
            
            if (p.position.y <= 0) { 
                p.position.y = 0; p.velocity.y = -p.velocity.y; bounced = true; 
            }
            else if (p.position.y >= SCREEN_HEIGHT - p.radius*2) { 
                p.position.y = SCREEN_HEIGHT - p.radius*2; p.velocity.y = -p.velocity.y; bounced = true; 
            }

            SDL_FRect bulletRect = p.getRect();
            for (const auto& plat : platforms) {
                SDL_FRect platRect = plat.getRect();
                if (SDL_HasRectIntersectionFloat(&bulletRect, &platRect)) {
                    p.velocity.x = -p.velocity.x;
                    p.velocity.y = -p.velocity.y;
                    bounced = true;
                    break;
                }
            }

            if (bounced) {
                p.bounceCount++;
                if (p.bounceCount >= MAX_BOUNCES) p.active = false;
            }
        }
    }
};