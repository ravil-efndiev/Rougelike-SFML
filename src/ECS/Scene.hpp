#pragma once
#include "Entity.hpp"

class Scene {
public:
    Scene();
    ~Scene() = default;

    Entity newEntity();

private:
    std::vector<Entity> mEntities;
    static i32 sEntityCounter;
};
