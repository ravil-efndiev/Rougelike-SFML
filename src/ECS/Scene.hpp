#pragma once
#include "System.hpp"
#include "ComponentRegistry.hpp"

class Entity;
class Scene {
public:
    friend class Entity;

    Scene() = default;
    ~Scene() = default;

    Entity newEntity();
    Scene& addSystem(const System& system);

    void update();

private:
    std::vector<Entity> mEntities;
    std::vector<System> mSystems;
    ComponentRegistry mRegistry;
    static i32 sEntityCounter;
};
