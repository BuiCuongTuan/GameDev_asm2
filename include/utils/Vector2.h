#pragma once
#include <cmath>

struct Vector2 {
    float x, y;

    Vector2() : x(0), y(0) {}
    Vector2(float _x, float _y) : x(_x), y(_y) {}

    Vector2 operator+(const Vector2& v) const {
        return Vector2(x + v.x, y + v.y);
    }

    Vector2 operator-(const Vector2& v) const {
        return Vector2(x - v.x, y - v.y);
    }

    Vector2 operator*(float s) const {
        return Vector2(x * s, y * s);
    }

    Vector2 normalize() {
        float length = std::sqrt(x * x + y * y);
        if (length > 0) {
            return Vector2(x / length, y / length);
        }
        return *this;
    }
};