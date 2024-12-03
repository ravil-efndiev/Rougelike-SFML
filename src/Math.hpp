#pragma once
#include <include.hpp>

enum class MoveDirection {
    up, down, left, right
};

void normalize(sf::Vector2f& vec);

f32 distance(const sf::Vector2f& a, const sf::Vector2f& b);
