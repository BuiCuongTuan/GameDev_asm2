#include "../../include/systems/CollisionSystem.h"
#include "../../include/utils/Constants.h"
#include <cmath>

void CollisionSystem::update(Player& player, std::vector<Projectile>& bullets, const std::vector<Platform>& platforms) {
    if (player.hp <= 0) return;

    // 1. Va chạm giữa Người chơi và Màn hình / Platform
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

    // 2. Va chạm của Đạn
    for (auto& p : bullets) {
        if (!p.active) continue;
        if (p.existTime < 0.05f) continue; // Tránh trường hợp đạn vừa sinh ra đã va chạm người bắn

        SDL_FRect bulletRect = p.getRect();
        SDL_FRect playerRect = player.getRect();

        // Va chạm đạn với người chơi
        if (SDL_HasRectIntersectionFloat(&bulletRect, &playerRect)) {
            if (p.ownerId != player.id) {
                player.hp -= p.damage;
                if (player.hp < 0) player.hp = 0;
                p.active = false;
                continue; 
            }
        }

        // PHẢN XẠ VỚI 4 CẠNH MÀN HÌNH
        // Tăng bounceCount ngay lúc va chạm và kiểm tra giới hạn nảy
        if (p.position.x <= 0) { 
            p.position.x = 0.1f; p.velocity.x = -p.velocity.x; 
            p.bounceCount++; if (p.bounceCount >= MAX_BOUNCES) p.active = false;
        } else if (p.position.x >= SCREEN_WIDTH - p.radius * 2) { 
            p.position.x = (SCREEN_WIDTH - p.radius * 2) - 0.1f; p.velocity.x = -p.velocity.x; 
            p.bounceCount++; if (p.bounceCount >= MAX_BOUNCES) p.active = false;
        }
        
        // Nếu đạn đã vô hiệu hóa (hết số lần nảy ở trục X), bỏ qua kiểm tra trục Y
        if (!p.active) continue;

        if (p.position.y <= 0) { 
            p.position.y = 0.1f; p.velocity.y = -p.velocity.y; 
            p.bounceCount++; if (p.bounceCount >= MAX_BOUNCES) p.active = false;
        } else if (p.position.y >= SCREEN_HEIGHT - p.radius * 2) { 
            p.position.y = (SCREEN_HEIGHT - p.radius * 2) - 0.1f; p.velocity.y = -p.velocity.y; 
            p.bounceCount++; if (p.bounceCount >= MAX_BOUNCES) p.active = false;
        }

        if (!p.active) continue;

        // PHẢN XẠ VỚI PLATFORM
        for (const auto& plat : platforms) {
            // Tính tâm của viên đạn và nền tảng
            float bCenterX = p.position.x + p.radius;
            float bCenterY = p.position.y + p.radius;
            float pCenterX = plat.x + plat.width / 2.0f;
            float pCenterY = plat.y + plat.height / 2.0f;

            // Tính vector khoảng cách giữa 2 tâm
            float dx = bCenterX - pCenterX;
            float dy = bCenterY - pCenterY;

            // Tính độ lún (overlap) trên từng trục (Nếu < 0 tức là đang lún vào nhau)
            float intersectX = std::abs(dx) - (p.radius + plat.width / 2.0f);
            float intersectY = std::abs(dy) - (p.radius + plat.height / 2.0f);

            // Có va chạm khi cả 2 trục đều lún
            if (intersectX < 0.0f && intersectY < 0.0f) {
                // Ưu tiên đẩy đạn ra theo trục có độ lún ít hơn
                if (intersectX > intersectY) {
                    // Va chạm ngang (trái/phải)
                    if (dx > 0) { 
                        p.position.x -= intersectX; // Đẩy sang phải
                        p.velocity.x = std::abs(p.velocity.x); // ÉP vận tốc hướng sang phải
                    } else { 
                        p.position.x += intersectX; // Đẩy sang trái
                        p.velocity.x = -std::abs(p.velocity.x); // ÉP vận tốc hướng sang trái
                    }
                } else {
                    // Va chạm dọc (trên/dưới)
                    if (dy > 0) { 
                        p.position.y -= intersectY; // Đẩy xuống dưới
                        p.velocity.y = std::abs(p.velocity.y); // ÉP vận tốc hướng xuống dưới
                    } else { 
                        p.position.y += intersectY; // Đẩy lên trên
                        p.velocity.y = -std::abs(p.velocity.y); // ÉP vận tốc hướng lên trên
                    }
                }
                
                // Tăng số lần nảy trực tiếp và kiểm tra giới hạn
                p.bounceCount++;
                if (p.bounceCount >= MAX_BOUNCES) {
                    p.active = false;
                }
                
                // Thoát vòng lặp platform ngay lập tức để tránh 1 viên đạn chạm 2 platform trong 1 frame
                break; 
            }
        }
        // ------------------------------------------------
    }
}