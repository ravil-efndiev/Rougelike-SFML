#pragma once
#include "System.hpp"
#include "ComponentRegistry.hpp"

class Entity;
class Scene {
public:
    friend class Entity;

    Scene();
    ~Scene();

    Entity newEntity(const std::string& name = "", const sf::Vector2f& initialPosition = { 0.f, 0.f });
    Scene& addSystem(const System& system);
    Scene& addEventSystem(const EventSystem& system);

    void update();
    void onEvent(const sf::Event& event);

    std::vector<Entity> getEntities() const;

private:
    std::vector<Entity> mEntities;
    std::vector<System> mSystems;
    std::vector<EventSystem> mEvtSystems;
    ComponentRegistry mRegistry;
    static i32 sEntityCounter;
};
