#pragma once
#include "System.hpp"
#include "ComponentRegistry.hpp"

class Entity;
class Scene {
public:
    friend class Entity;

    Scene();
    ~Scene();

    Entity newEntity(const sf::Vector2f& initialPosition = { 0.f, 0.f });
    Scene& addSystem(const System& system);

    void update();

    std::vector<Entity> getEntities() const;

private:
    std::vector<Entity> mEntities;
    std::vector<System> mSystems;
    ComponentRegistry mRegistry;
    static i32 sEntityCounter;
};
