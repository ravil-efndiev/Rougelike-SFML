#pragma once
#include <Components/Collider.hpp>
#include <Entity.hpp>

struct ColliderList {
    ColliderList(bool dynamic) : dynamic(dynamic) {}

    // if collider positions need to be updated constantly or no
    bool dynamic = false;
    std::vector<Collider> colliders;
};

void colliderListPositionSystem(const std::vector<Entity>& entities);
