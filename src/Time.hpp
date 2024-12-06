#pragma once
#include "include.hpp"

class Time {
public:
    static void update();
    static f32 dt();
    static sf::Time sfDt();

private: 
    static sf::Clock sClock;
    static sf::Time sDeltaTime;
};
