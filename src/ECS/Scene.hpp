#pragma once
#include "System.hpp"
#include "ComponentStorage.hpp"

class Scene {
public:
    friend class Entity;

    Scene();
    ~Scene() = default;

    Entity newEntity();
    Scene& addSystem(const System& system);

    void update();

private:
    std::vector<Entity> mEntities;
    std::vector<System> mSystems;
    ComponentStorage mStorage;
    static i32 sEntityCounter;
};
