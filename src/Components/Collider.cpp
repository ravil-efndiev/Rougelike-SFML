#include "Collider.hpp"

sf::Vector2f centerColliderToSprite(const Collider* coll, const Transform* spriteTf, const Sprite* sprite) {
    return {
        sprite->centerPosition(spriteTf->position).x - coll->bounds.width / 2.f,
        sprite->centerPosition(spriteTf->position).y - coll->bounds.height / 2.f
    };
}
