#pragma once

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// player
const float GRAVITY = 2000.0f;
const float JUMP_FORCE = -900.0f;
const float MOVE_SPEED = 400.0f;

//Đạn
const int MAX_BOUNCES = 5;
const float BULLET_SPEED = 800.0f;
const int MAX_PROJECTILES = 20;

// Charge
const float MAX_CHARGE_TIME = 1.0f;
const float MIN_BULLET_RADIUS = 8.0f;
const float MAX_BULLET_RADIUS = 25.0f;

const float MAX_MANA = 100.0f; 
const float MANA_REGEN = 15.0f; 
const float MANA_COST = 20.0f;

// Aiming
const float ROT_TIME = 2.0f;
const float AIM_LENGTH = 60.0f;
const float PI = 3.14159265f;