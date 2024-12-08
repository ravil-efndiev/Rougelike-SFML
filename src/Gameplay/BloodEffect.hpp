#pragma once
#include <Entity.hpp>
#include <Components/Transform.hpp>

struct BloodEffect {
    bool active = false;
};

Entity initBloodEffect(Scene& scene);
void bloodEffectSystem(const std::vector<Entity>& entities);
