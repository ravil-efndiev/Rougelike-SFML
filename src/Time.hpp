#pragma once
#include "include.hpp"

class Time {
public:
    static void update();
    static f32 dt();

private: 
    static sf::Clock sClock;
    static f32 sDeltaTime;
};
