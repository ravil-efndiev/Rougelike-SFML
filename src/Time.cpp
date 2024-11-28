#include "Time.hpp"

sf::Clock Time::sClock {};
f32 Time::sDeltaTime {};

void Time::update() {
    sDeltaTime = sClock.restart().asSeconds();
}

f32 Time::dt() {
    return sDeltaTime;
}
