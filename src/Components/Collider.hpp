#pragma once
#include <include.hpp>
#include "Transform.hpp"
#include "Sprite.hpp"

struct Collider {
    Collider(
        const sf::Vector2f& size, const sf::Vector2f& offset = {0.f, 0.f},
        bool debugRender = false, bool solid = false) 
        : bounds(offset, size), offset(offset), debugRender(debugRender), solid(solid) {}

    sf::Vector2f offset;
    sf::FloatRect bounds;
    bool active = true;
    bool solid;
    bool debugRender;

    bool intercepts(const Collider& other) const {
        return bounds.intersects(other.bounds);
    }
};

sf::Vector2f centerColliderToSprite(const Collider* coll, const Transform* spriteTf, const Sprite* sprite);
