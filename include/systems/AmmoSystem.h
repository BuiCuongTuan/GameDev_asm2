#pragma once
#include <vector>
#include "../entities/Player.h"
#include "../entities/Projectile.h"

class AmmoSystem {
public:
    void update(Player& player, float deltaTime, std::vector<Projectile>& bullets);
};