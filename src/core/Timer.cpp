#include "../../include/core/Timer.h"

Timer::Timer(float maxTime) : currentTime(maxTime), maxTime(maxTime) {}

void Timer::update(float deltaTime) {
    if (currentTime > 0) {
        currentTime -= deltaTime;
        if (currentTime < 0) currentTime = 0;
    }
}

float Timer::getCurrentTime() const {
    return currentTime;
}

bool Timer::isTimeUp() const {
    return currentTime <= 0;
}

void Timer::reset() {
    currentTime = maxTime;
}