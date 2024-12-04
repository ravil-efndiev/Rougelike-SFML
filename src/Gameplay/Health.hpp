#pragma once
#include <include.hpp>

struct Health {
    Health(const std::function<void(i32)>& onTakeDamage) 
        : onTakeDamage(onTakeDamage) {}

    i32 health = 100;
    std::function<void(i32)> onTakeDamage;

    void damage(i32 damage) {
        health -= damage;
        onTakeDamage(health);
    }
};

