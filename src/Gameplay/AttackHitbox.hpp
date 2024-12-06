#pragma once
#include <Entity.hpp>
#include <Components/Sprite.hpp>
#include <Components/Animator.hpp>
#include <Components/Collider.hpp>
#include <Components/Transform.hpp>
#include "Player.hpp"

struct AttackHitbox { 
    AttackHitbox(f32 width, f32 height, i32 damage) 
        : width(width), height(height), damage(damage) {}

    enum AttackTarget { 
        player, enemy 
    } targets = enemy;

    f32 width;
    f32 height;

    i32 damage;

    std::vector<EntityId> entitiesTookDamage;
    bool entityTookDamage(EntityId id) const {
        return std::find(entitiesTookDamage.begin(), entitiesTookDamage.end(), id) != entitiesTookDamage.end();
    }
};

void attackHitboxSystem(const std::vector<Entity>& entities);

void spawnAttackHitbox(
    MoveDirection direction,
    AttackHitbox* atkHitbox,
    Collider* collider,
    Transform* colliderTf,
    const Transform* holderTf,
    const Sprite* holderSprite
);

void turnOffHitbox(Collider* hitboxCollider);
