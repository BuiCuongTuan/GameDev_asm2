#include "../../include/core/Game.h"
#include "../../include/utils/Constants.h"
#include "../../include/systems/InputSystem.h"
#include "../../include/systems/AmmoSystem.h"
#include "../../include/systems/PhysicSystem.h"
#include "../../include/systems/CollisionSystem.h"
#include "../../include/systems/RenderSystem.h"
#include <iostream>
#include <algorithm>
#include <cstdio> 

InputSystem inputSys;
AmmoSystem ammoSys;
PhysicSystem physicsSys;
CollisionSystem collisionSys;
RenderSystem renderSys;

Game::Game() : gameWindow(nullptr), isRunning(false), 
                player1(nullptr), player2(nullptr), 
                texProjectile(nullptr), 
                texPlayer1(nullptr), texPlayer2(nullptr),
                texBackground(nullptr),
                wavData(nullptr), audioStream(nullptr),
                matchTimer(90.0f) {}

Game::~Game() { clean(); }

bool Game::init(const char* title, int width, int height) {
    gameWindow = new Window();
    if (!gameWindow->init(title, width, height)) return false;

    SDL_Renderer* renderer = gameWindow->getRenderer();

    // --- Tải hình nền ---
    SDL_Surface* surfBg = SDL_LoadBMP("assets/textures/background.bmp");
    if (surfBg) {
        texBackground = SDL_CreateTextureFromSurface(renderer, surfBg);
        SDL_DestroySurface(surfBg);
    }

    // --- TẢI VÀ PHÁT NHẠC NỀN (BGM) ---
    wavData = nullptr; // Đảm bảo an toàn trước khi load
    SDL_LoadWAV("assets/sounds/08 - Lake.wav", &wavSpec, &wavData, &wavLength);
    if (wavData) {
        // Mở luồng âm thanh mặc định để phát nhạc
        audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &wavSpec, nullptr, nullptr);
        if (audioStream) {
            // Nạp dữ liệu nhạc vào luồng
            SDL_PutAudioStreamData(audioStream, wavData, wavLength);
            // Kích hoạt thiết bị âm thanh để bắt đầu phát
            SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(audioStream)); 
        }
    } else {
        SDL_Log("Khong the load bgm.wav: %s", SDL_GetError());
    }

    // --- Tải hình đạn ---
    SDL_Surface* surfBullet = SDL_LoadBMP("assets/textures/projectile.bmp");
    if (surfBullet) {
        SDL_SetSurfaceColorKey(surfBullet, true, SDL_MapSurfaceRGB(surfBullet, 0, 0, 0));
        texProjectile = SDL_CreateTextureFromSurface(renderer, surfBullet);
        SDL_DestroySurface(surfBullet);
    }

    // --- Tải hình nhân vật (DÙNG CHUNG 1 ẢNH GỐC CHO CẢ 2) ---
    SDL_Surface* surfP1 = SDL_LoadBMP("assets/textures/player.bmp");
    if (surfP1) {
        SDL_SetSurfaceColorKey(surfP1, true, SDL_MapSurfaceRGB(surfP1, 0, 0, 0));
        texPlayer1 = SDL_CreateTextureFromSurface(renderer, surfP1);
        texPlayer2 = SDL_CreateTextureFromSurface(renderer, surfP1); 
        SDL_DestroySurface(surfP1);
    }

    // ==========================================
    // KHỞI TẠO CÁC HITBOX (PLATFORMS) TÀNG HÌNH
    // ==========================================
    platforms.push_back({0, 480, 800, 150}); 
    platforms.push_back({0, 290, 340, 50});
    platforms.push_back({600, 290, 280, 50});

    // Vị trí xuất hiện của 2 pháp sư
    player1 = new Player(1, 100, 100); 
    player2 = new Player(2, 700, 100); 

    isRunning = true;
    return true;
}

void Game::handleEvents(SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) isRunning = false;
}

void Game::update(float deltaTime) {
    if (!player1 || !player2) return;
    
    if (player1->hp <= 0 || player2->hp <= 0 || matchTimer.isTimeUp()) return;

    matchTimer.update(deltaTime);

    // --- LOOP NHẠC NỀN ---
    // Nếu luồng âm thanh đã phát cạn kiệt dữ liệu, ta lại nhét data vào để bài nhạc phát lại từ đầu
    if (audioStream && SDL_GetAudioStreamAvailable(audioStream) == 0) {
        SDL_PutAudioStreamData(audioStream, wavData, wavLength);
    }
    // ---------------------

    int numKeys;
    const bool* keys = SDL_GetKeyboardState(&numKeys);

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), 
        [](const Projectile& p) { return !p.active; }), bullets.end());

    inputSys.update(*player1, deltaTime, keys);
    ammoSys.update(*player1, deltaTime, bullets);
    inputSys.update(*player2, deltaTime, keys);
    ammoSys.update(*player2, deltaTime, bullets);

    physicsSys.updatePlayer(*player1, deltaTime);
    physicsSys.updatePlayer(*player2, deltaTime);
    physicsSys.updateBullets(bullets, deltaTime); 

    collisionSys.update(*player1, bullets, platforms);
    collisionSys.update(*player2, bullets, platforms);

    float rotSpeed = 360.0f / ROT_TIME;
    player1->aimAngle += rotSpeed * deltaTime;
    player2->aimAngle += rotSpeed * deltaTime;
}

void Game::render() {
    if (!gameWindow) return;
    gameWindow->clear();
    SDL_Renderer* renderer = gameWindow->getRenderer();

    // --- VẼ HÌNH NỀN ---
    if (texBackground) {
        SDL_RenderTexture(renderer, texBackground, nullptr, nullptr); 
    }

    // --- VẼ TIMER ĐẾM NGƯỢC ---
    int timeLeft = (int)matchTimer.getCurrentTime();
    char timeStr[16];
    snprintf(timeStr, sizeof(timeStr), "%02d", timeLeft);

    SDL_SetRenderScale(renderer, 4.0f, 4.0f);
    SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255); 
    
    float textX = (400.0f / 4.0f) - 8.0f; 
    float textY = 15.0f / 4.0f; 
    SDL_RenderDebugText(renderer, textX, textY, timeStr);
    
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    // -------------------------

    if (player1 && player2) {
        // =========================================================
        // VẼ HÀO QUANG (AURA) DƯỚI CHÂN ĐỂ PHÂN BIỆT 2 NGƯỜI CHƠI
        // =========================================================
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // Hào quang Player 1 (Hệ Lửa - Đỏ)
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 180); // Đỏ bán trong suốt
        SDL_FRect aura1 = { player1->position.x - 5, player1->position.y + player1->height - 10, player1->width + 10, 15 };
        SDL_RenderFillRect(renderer, &aura1);

        // Hào quang Player 2 (Hệ Thủy - Xanh Lam)
        SDL_SetRenderDrawColor(renderer, 50, 150, 255, 180); // Xanh lam bán trong suốt
        SDL_FRect aura2 = { player2->position.x - 5, player2->position.y + player2->height - 10, player2->width + 10, 15 };
        SDL_RenderFillRect(renderer, &aura2);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        // =========================================================

        // Vẽ nhân vật (Nhân vật sẽ đứng đè lên trên lớp hào quang)
        renderSys.render(renderer, *player1, *player2, bullets, platforms, texProjectile, texPlayer1, texPlayer2);
        
        // Màn hình Game Over / Hết giờ
        if (player1->hp <= 0 || player2->hp <= 0 || matchTimer.isTimeUp()) {
             SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
             SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); 
             SDL_FRect overlay = {0, 0, 800, 600};
             SDL_RenderFillRect(renderer, &overlay);
             
             SDL_SetRenderScale(renderer, 4.0f, 4.0f);
             if (matchTimer.isTimeUp() && player1->hp > 0 && player2->hp > 0) {
                 SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                 SDL_RenderDebugText(renderer, (400.0f / 4.0f) - 32.0f, (300.0f / 4.0f) - 4.0f, "TIME UP!");
             } else {
                 SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
                 SDL_RenderDebugText(renderer, (400.0f / 4.0f) - 36.0f, (300.0f / 4.0f) - 4.0f, "GAME OVER");
             }
             SDL_SetRenderScale(renderer, 1.0f, 1.0f);
        }
    }
    
    gameWindow->display();
}

void Game::clean() {
    // --- Dọn dẹp âm thanh ---
    if (audioStream) SDL_DestroyAudioStream(audioStream);
    if (wavData) SDL_free(wavData); // Hàm free đặc biệt của SDL
    // ------------------------
    if (texBackground) SDL_DestroyTexture(texBackground);
    if (texBackground) SDL_DestroyTexture(texBackground);
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