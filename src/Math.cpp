#include "Math.hpp"

void normalize(sf::Vector2f& vec) {
    f32 magnitude = sqrt(pow(vec.x, 2) + pow(vec.y, 2));
    if (magnitude != 0) vec /= magnitude;
}

f32 distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
