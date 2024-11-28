#pragma once
#include <include.hpp>

struct Transform {
    Transform(const sf::Vector2f& position, const sf::Vector2f& scale, f32 rotation)
        : position(position), scale(scale), rotation(rotation) {}

    sf::Vector2f position;
    sf::Vector2f scale;
    f32 rotation;
};
