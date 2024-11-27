#include "SpriteSystems.hpp"

void spriteTransformSystem(const std::vector<Entity>& entities) {
    for (const Entity& entity : entities) {
        if (!entity.has<Sprite>() || !entity.has<Transform>()) continue;

        auto* transform = entity.get<Transform>();
        auto* sprite = entity.get<Sprite>();

        sprite->sprite.setPosition(transform->position);
        sprite->sprite.setRotation(transform->rotation);
        sprite->sprite.setScale(transform->scale);
    }
}
