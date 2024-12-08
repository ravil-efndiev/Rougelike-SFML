#pragma once
#include <Entity.hpp>

struct Health {
    Health(const std::function<void(i32, i32)>& onTakeDamage, i32 health, i32 poise) 
        : onTakeDamage(onTakeDamage), health(health), poise(poise), maxPoise(poise) {}

    i32 health;
    i32 poise, maxPoise;
    std::function<void(i32, i32)> onTakeDamage;

    f32 poiseRegen = 100.f;
    f32 poiseRegenTimer {};

    void damage(i32 damage, i32 poiseDamage);
};

void poiseRegenSystem(const std::vector<Entity>& entities);
