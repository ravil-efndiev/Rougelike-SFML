#include "ColliderList.hpp"
#include <Components/Transform.hpp>
#include <Components/Sprite.hpp>

void colliderListPositionSystem(const std::vector<Entity>& entities) {
    for (const auto& entity : entities) {
        if (!entity.has<ColliderList>() || !entity.has<Transform>()) continue;

        auto* collList = entity.get<ColliderList>();
        if (!collList->dynamic) continue;

        auto* tf = entity.get<Transform>();
        Sprite* sprite = entity.has<Sprite>() ? entity.get<Sprite>() : nullptr;

        for (Collider& coll : collList->colliders) {
            if (sprite) {
                coll.bounds.left = sprite->centerPosition(tf->position).x - coll.bounds.width / 2.f + coll.offset.x;
                coll.bounds.top = sprite->centerPosition(tf->position).y - coll.bounds.height / 2.f + coll.offset.y;
            }
            else {
                coll.bounds.left = tf->position.x + coll.offset.x;
                coll.bounds.top  = tf->position.y + coll.offset.y;
            }
        }
    }
}
