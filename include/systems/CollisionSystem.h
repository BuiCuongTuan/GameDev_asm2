#pragma once
#include <vector>
#include "../entities/Player.h"
#include "../entities/Projectile.h"
#include "../entities/Platform.h"

class CollisionSystem {
public:
    void update(Player& player, std::vector<Projectile>& bullets, const std::vector<Platform>& platforms);
};