#include "Time.hpp"

sf::Clock Time::sClock {};
sf::Time Time::sDeltaTime {};

void Time::update() {
    sDeltaTime = sClock.restart();
}

f32 Time::dt() {
    return sDeltaTime.asSeconds();
}

sf::Time Time::sfDt() {
    return sDeltaTime;
}
