#pragma once

class Timer {
public:
    Timer(float maxTime = 90.0f);
    void update(float deltaTime);
    float getCurrentTime() const;
    bool isTimeUp() const;
    void reset();

private:
    float currentTime;
    float maxTime;
};