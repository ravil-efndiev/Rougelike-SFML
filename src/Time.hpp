#pragma once
#include "include.hpp"

class Time {
public:
    static void update();
    static f32 dt();
    static sf::Time sfDt();
    static f32 fixedDt() { return 1.f / 60.f; }

private: 
    static sf::Clock sClock;
    static sf::Time sDeltaTime;
};
