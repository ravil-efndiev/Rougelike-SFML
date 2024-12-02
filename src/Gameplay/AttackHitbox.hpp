#pragma once
#include <Entity.hpp>

struct AttackHitbox { 
    enum AttackTarget { 
        player, enemy 
    } targets = enemy;
};

void attackHitboxSystem(const std::vector<Entity>& entities);
