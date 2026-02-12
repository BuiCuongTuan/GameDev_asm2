// --- QUAN TRỌNG: 2 dòng này giúp SDL3 tự tạo hàm WinMain ---
#define SDL_MAIN_USE_CALLBACKS 1 
#include <SDL3/SDL_main.h> 
// ----------------------------------------------------------

#include <SDL3/SDL.h>
#include "../include/core/Game.h" // Đảm bảo đường dẫn đúng với thư mục của bạn

// Hàm khởi tạo: Chạy 1 lần khi game bắt đầu
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    // Khởi tạo Subsystem Video, Audio, v.v. nếu cần
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        return SDL_APP_FAILURE;
    }

    Game* game = new Game();
    *appstate = game;

    if (game->init("Magic Arena", 800, 600)) {
        return SDL_APP_CONTINUE;
    }
    
    return SDL_APP_FAILURE;
}

// Hàm vòng lặp: Chạy liên tục (tương đương update + render)
SDL_AppResult SDL_AppIterate(void *appstate) {
    Game* game = (Game*)appstate;

    // Tính DeltaTime (thời gian giữa 2 khung hình)
    static Uint64 lastTime = SDL_GetTicks();
    Uint64 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    
    // Giới hạn deltaTime để tránh lỗi xuyên tường khi máy lag
    if (deltaTime > 0.05f) deltaTime = 0.05f;

    // Gọi Update và Render của Game
    game->update(deltaTime);
    game->render();

    return SDL_APP_CONTINUE; 
}

// Hàm xử lý sự kiện: Bàn phím, chuột, đóng cửa sổ...
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    Game* game = (Game*)appstate;
    
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; // Thoát game
    }
    
    game->handleEvents(event);
    return SDL_APP_CONTINUE;
}

// Hàm dọn dẹp: Chạy khi game tắt
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (appstate) {
        Game* game = (Game*)appstate;
        delete game;
    }
}