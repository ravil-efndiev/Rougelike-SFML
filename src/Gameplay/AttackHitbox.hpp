#pragma once
#include <Entity.hpp>
#include <Components/Sprite.hpp>
#include <Components/Animator.hpp>
#include <Components/Collider.hpp>
#include <Components/Transform.hpp>
#include "Player.hpp"

struct AttackHitbox { 
    AttackHitbox(f32 width, f32 height) : width(width), height(height) {}

    enum AttackTarget { 
        player, enemy 
    } targets = enemy;

    f32 width;
    f32 height;

    bool tookDamage = false;
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
