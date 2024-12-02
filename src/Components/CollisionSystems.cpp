#include "CollisionSystems.hpp"

void colliderPositionSystem(const std::vector<Entity>& entities) {
    for (const auto& entity : entities) {
        if (!entity.has<Collider>() || !entity.has<Transform>()) continue;

        auto* coll = entity.get<Collider>();
        if (!coll->active) continue;

        auto* tf = entity.get<Transform>();

        if (entity.has<Sprite>()) {
            auto* sprite = entity.get<Sprite>();
            coll->bounds.left = sprite->centerPosition(tf->position).x - coll->bounds.width / 2.f;
            coll->bounds.top = sprite->centerPosition(tf->position).y - coll->bounds.height / 2.f;
        }
        else {
            coll->bounds.left = tf->position.x;
            coll->bounds.top = tf->position.y;
        }
    }
}
