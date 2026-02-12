#include "../../include/entities/Projectile.h"

Projectile::Projectile(float x, float y, float velX, float velY, float r, int owner)
    : position(x, y), 
      velocity(velX, velY), 
      radius(r), 
      ownerId(owner), 
      active(true), 
      bounceCount(0),
      existTime(0.0f)
{
}

SDL_FRect Projectile::getRect() const { 
    return { position.x, position.y, radius * 2, radius * 2 }; 
}