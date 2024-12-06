#pragma once
#include <include.hpp>

struct Health {
    Health(const std::function<void(i32)>& onTakeDamage, i32 health) 
        : onTakeDamage(onTakeDamage), health(health) {}

    i32 health;
    std::function<void(i32)> onTakeDamage;

    void damage(i32 damage) {
        health -= damage;
        onTakeDamage(health);
    }
};

