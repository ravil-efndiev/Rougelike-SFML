#pragma once
#include <include.hpp>
#include "Transform.hpp"
#include "Sprite.hpp"

struct Collider {
    Collider(const sf::Vector2f& size, bool solid = false) 
        : bounds({0.f, 0.f}, size), solid(solid) {}

    sf::FloatRect bounds;
    bool active = true;
    bool solid = false;
    bool debugRender = false;

    bool intercepts(const Collider& other) {
        return bounds.intersects(other.bounds);
    }
};

sf::Vector2f centerColliderToSprite(const Collider* coll, const Transform* spriteTf, const Sprite* sprite);