#include "Math.hpp"

void normalize(sf::Vector2f& vec) {
    f32 magnitude = sqrt(pow(vec.x, 2) + pow(vec.y, 2));
    if (magnitude != 0) vec /= magnitude;
}

f32 distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void moveTowards(sf::Vector2f &current, const sf::Vector2f &target, f32 step) {
    sf::Vector2f dir = target - current;
    normalize(dir);
    current += step * dir;
}

std::mt19937 Random::sGenerator;

void Random::init() {
    std::random_device rd;
    sGenerator = std::mt19937(rd());
}

f32 Random::rangef(f32 min, f32 max) {
    std::uniform_real_distribution<> dist (min, max);
    return static_cast<f32>(dist(sGenerator));
}

i32 Random::rangei(i32 min, i32 max) {
    std::uniform_int_distribution<> dist (min, max);
    return dist(sGenerator);
}
