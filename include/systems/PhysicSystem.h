#pragma once
#include <vector>
#include "../entities/Player.h"
#include "../entities/Projectile.h"
#include "../utils/Constants.h"

class PhysicSystem {
public:
    void update(Player& player, std::vector<Projectile>& bullets, float deltaTime) {
        player.velocity.y += GRAVITY * deltaTime;
        player.position.x += player.velocity.x * deltaTime;
        player.position.y += player.velocity.y * deltaTime;

        for (auto& p : bullets) {
            if (!p.active) continue;
            
            p.existTime += deltaTime;

            p.position.x += p.velocity.x * deltaTime;
            p.position.y += p.velocity.y * deltaTime;
        }
    }
};