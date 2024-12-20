#include "AttackHitbox.hpp"
#include "Health.hpp"
#include "ColliderList.hpp"

void attackHitboxSystem(const std::vector<Entity>& entities) {
    for (const auto& entity : entities) {
        if (!entity.has<AttackHitbox>() || !entity.has<Collider>()) continue;

        auto* coll = entity.get<Collider>();
        auto* hitbox = entity.get<AttackHitbox>();

        if (!coll->active) continue;
        if (hitbox->targets == AttackHitbox::enemy) {
            std::vector<Entity> enemies = findEntitiesByName(entities, "enemy");
            for (const auto& enemy : enemies) {
                auto* enemyColl = enemy.get<Collider>();

                if (coll->intercepts(*enemyColl) && !hitbox->entityTookDamage(enemy.id())) {
                    enemy.get<Health>()->damage(hitbox->damage, 20);
                    hitbox->entitiesTookDamage.push_back(enemy.id());
                }
            }
        }
        else if (hitbox->targets == AttackHitbox::player) {
            Entity player = findEntityByName(entities, "player");
            auto* collList = player.get<ColliderList>();
            Collider playerDmgColl = collList->colliders[PLAYER_DAMAGE_COLLIDER];
            
            if (coll->intercepts(playerDmgColl) && !hitbox->entityTookDamage(player.id())) {
                player.get<Health>()->damage(hitbox->damage, 20);
                hitbox->entitiesTookDamage.push_back(player.id());
            }
        }
    }
}

void spawnAttackHitbox(
    MoveDirection direction,
    AttackHitbox* atkHitbox,
    Collider* collider,
    Transform* colliderTf,
    const Transform* holderTf,
    const Sprite* holderSprite
) {
    collider->active = true;
    atkHitbox->entitiesTookDamage.clear();
    switch (direction) {
    case MoveDirection::left:
        collider->bounds.width = atkHitbox->width;
        collider->bounds.height = atkHitbox->height;
        colliderTf->position = centerColliderToSprite(collider, holderTf, holderSprite);
        colliderTf->position.x -= atkHitbox->width;
        break;
    case MoveDirection::right:
        collider->bounds.width = atkHitbox->width;
        collider->bounds.height = atkHitbox->height;
        colliderTf->position = centerColliderToSprite(collider, holderTf, holderSprite);
        colliderTf->position.x += atkHitbox->width;
        break;
    case MoveDirection::up:
        collider->bounds.width = atkHitbox->height;
        collider->bounds.height = atkHitbox->width;
        colliderTf->position = centerColliderToSprite(collider, holderTf, holderSprite);
        colliderTf->position.y -= atkHitbox->width;
        break;
    case MoveDirection::down:
        collider->bounds.width = atkHitbox->height;
        collider->bounds.height = atkHitbox->width;
        colliderTf->position = centerColliderToSprite(collider, holderTf, holderSprite);
        colliderTf->position.y += atkHitbox->width;
        break;
    }
}

void turnOffHitbox(Collider *hitboxCollider) {
    hitboxCollider->active = false;
    hitboxCollider->bounds.width = 0.f;
    hitboxCollider->bounds.height = 0.f;
}
