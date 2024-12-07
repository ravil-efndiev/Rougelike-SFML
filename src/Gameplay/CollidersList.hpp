#pragma once
#include <Components/Collider.hpp>

struct CollidersList {
    // if collider positions need to be updated constantly or no
    bool dynamic = false;
    std::vector<Collider> colliders;
};

