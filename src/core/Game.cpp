#include "../../include/core/Game.h"
#include "../../include/utils/Constants.h"
#include "../../include/systems/InputSystem.h"
#include "../../include/systems/AmmoSystem.h"
#include "../../include/systems/PhysicSystem.h"
#include "../../include/systems/CollisionSystem.h"
#include "../../include/systems/RenderSystem.h"
#include <iostream>
#include <algorithm>

InputSystem inputSys;
AmmoSystem ammoSys;
PhysicSystem physicsSys;
CollisionSystem collisionSys;
RenderSystem renderSys;

Game::Game() : gameWindow(nullptr), isRunning(false), 
               player1(nullptr), player2(nullptr), 
               texProjectile(nullptr), 
               texPlayer1(nullptr), texPlayer2(nullptr) {}

Game::~Game() { clean(); }

bool Game::init(const char* title, int width, int height) {
    gameWindow = new Window();
    if (!gameWindow->init(title, width, height)) return false;

    SDL_Renderer* renderer = gameWindow->getRenderer();

    SDL_Surface* surfBullet = SDL_LoadBMP("assets/textures/projectile.bmp");
    if (surfBullet) {
        SDL_SetSurfaceColorKey(surfBullet, true, SDL_MapSurfaceRGB(surfBullet, 0, 0, 0));
        texProjectile = SDL_CreateTextureFromSurface(renderer, surfBullet);
        SDL_DestroySurface(surfBullet);
    }

    SDL_Surface* surfP1 = SDL_LoadBMP("assets/textures/player1.bmp");
    if (surfP1) {
        SDL_SetSurfaceColorKey(surfP1, true, SDL_MapSurfaceRGB(surfP1, 0, 0, 0));
        texPlayer1 = SDL_CreateTextureFromSurface(renderer, surfP1);
        SDL_DestroySurface(surfP1);
    } else {
        std::cout << "Khong tim thay player1.bmp" << std::endl;
    }

    SDL_Surface* surfP2 = SDL_LoadBMP("assets/textures/player2.bmp");
    if (surfP2) {
        SDL_SetSurfaceColorKey(surfP2, true, SDL_MapSurfaceRGB(surfP2, 0, 0, 0));
        texPlayer2 = SDL_CreateTextureFromSurface(renderer, surfP2);
        SDL_DestroySurface(surfP2);
    } else {
        std::cout << "Khong tim thay player2.bmp" << std::endl;
    }

    platforms.push_back({0, 550, 800, 50}); 
    platforms.push_back({100, 400, 200, 20});
    platforms.push_back({500, 400, 200, 20});
    platforms.push_back({300, 250, 200, 20});

    player1 = new Player(1, 150, 300);
    player2 = new Player(2, 600, 300);

    isRunning = true;
    return true;
}

void Game::handleEvents(SDL_Event* event) {}

void Game::update(float deltaTime) {
    int numKeys;
    const bool* keys = SDL_GetKeyboardState(&numKeys);

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), 
        [](const Projectile& p) { return !p.active; }), bullets.end());

    if (player1) {
        inputSys.update(*player1, deltaTime, keys);
        
        ammoSys.update(*player1, deltaTime, bullets);

        physicsSys.update(*player1, bullets, deltaTime);
        collisionSys.update(*player1, bullets, platforms);

        float rotSpeed = 360.0f / ROT_TIME;
        player1->aimAngle += rotSpeed * deltaTime;
        if(player1->aimAngle >= 360) player1->aimAngle -= 360;
    }

    if (player2) {
        inputSys.update(*player2, deltaTime, keys);
        
        ammoSys.update(*player2, deltaTime, bullets);

        physicsSys.update(*player2, bullets, deltaTime);
        collisionSys.update(*player2, bullets, platforms);

        float rotSpeed = 360.0f / ROT_TIME;
        player2->aimAngle += rotSpeed * deltaTime;
        if(player2->aimAngle >= 360) player2->aimAngle -= 360;
    }
}

void Game::render() {
    if (!gameWindow) return;
    gameWindow->clear();
    SDL_Renderer* renderer = gameWindow->getRenderer();

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    for (const auto& plat : platforms) {
        SDL_FRect rect = plat.getRect();
        SDL_RenderFillRect(renderer, &rect);
    }

    if (player1) renderSys.render(renderer, *player1, bullets, platforms, texProjectile, texPlayer1, texPlayer2);
    if (player2) renderSys.render(renderer, *player2, bullets, platforms, texProjectile, texPlayer1, texPlayer2);

    gameWindow->display();
}

void Game::clean() {
    if (texProjectile) SDL_DestroyTexture(texProjectile);
    if (texPlayer1) SDL_DestroyTexture(texPlayer1);
    if (texPlayer2) SDL_DestroyTexture(texPlayer2);

    if (player1) delete player1;
    if (player2) delete player2;
    if (gameWindow) {
        gameWindow->clean();
        delete gameWindow;
    }
}