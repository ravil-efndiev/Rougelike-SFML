#include "Health.hpp"
#include <Time.hpp>

void poiseRegenSystem(const std::vector<Entity>& entities) {
    for (const auto& entity : entities) {
        if (!entity.has<Health>()) continue;

        auto* health = entity.get<Health>();

        if (health->poise < health->maxPoise) {
            health->poiseRegenTimer += 100.f * Time::dt();
            if (health->poiseRegenTimer >= health->poiseRegen) {
                health->poise = health->maxPoise;               
            }
        }
    }
}

void Health::damage(i32 damage, i32 poiseDamage) {
    health -= damage;
    poise -= poiseDamage;
    poiseRegenTimer = 0;
    onTakeDamage(health, poise);
}
