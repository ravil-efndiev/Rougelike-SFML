#pragma once
#include <include.hpp>

enum class MoveDirection {
    up, down, left, right
};

static const std::unordered_map<MoveDirection, std::string> directionNames = {
    {MoveDirection::left, "left"},
    {MoveDirection::right, "right"},
    {MoveDirection::down, "down"},
    {MoveDirection::up, "up"},
};

void normalize(sf::Vector2f& vec);

f32 distance(const sf::Vector2f& a, const sf::Vector2f& b);

void moveTowards(sf::Vector2f& current, const sf::Vector2f& target, f32 step);

class Random {
public:
    static void init();
    static f32 rangef(f32 min, f32 max);
    static i32 rangei(i32 min, i32 max);

private:
    static std::mt19937 sGenerator;
};
