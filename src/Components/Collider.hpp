#pragma once
#include <include.hpp>

struct Collider {
    Collider(const sf::Vector2f& size, bool solid = false) 
        : bounds({0.f, 0.f}, size), solid(solid) {}

    sf::FloatRect bounds;
    bool solid;
    bool debugRender = false;

    bool intercepts(const Collider& other) {
        return bounds.intersects(other.bounds);
    }
};
