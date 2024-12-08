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
    void removeEntity(Entity entity);
    Scene& addSystem(const System& system);
    Scene& addFixedSystem(const System& system);
    Scene& addEventSystem(const EventSystem& system);

    void update();
    void tick();
    void onEvent(const sf::Event& event);
    void onMainLoopEnd();

    std::vector<Entity> entities() const;

private:
    std::vector<Entity> mEntities;
    std::vector<System> mSystems;
    std::vector<System> mFixedSystems;
    std::vector<EventSystem> mEvtSystems;
    ComponentRegistry mRegistry;

    std::vector<Entity> mRemovals;
    static i32 sEntityCounter;
};

Entity findEntityById(const std::vector<Entity>& entities, EntityId id);
Entity findEntityByName(const std::vector<Entity>& entities, const std::string& name);
std::vector<Entity> findEntitiesByName(const std::vector<Entity>& entities, const std::string& name);
