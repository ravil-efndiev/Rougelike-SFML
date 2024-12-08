#include "Scene.hpp"
#include "Entity.hpp"
#include <Components/SpriteSystems.hpp>
#include <Components/Transform.hpp>
#include <Components/Tag.hpp>

i32 Scene::sEntityCounter = 0;

Scene::Scene() {}
Scene::~Scene() = default;

Entity Scene::newEntity(const std::string& name, const sf::Vector2f& initialPosition)
{
    Entity newEntity (sEntityCounter++, this);
    newEntity.add<Transform>(initialPosition, sf::Vector2f(1.f, 1.f), 0.f);

    if (name.empty()) {
        newEntity.add<Tag>("Entity " + std::to_string(sEntityCounter));
    }
    else {
        newEntity.add<Tag>(name);
    }
    mEntities.push_back(newEntity);
    return newEntity;
}

void Scene::removeEntity(Entity entity) {
    mRemovals.push_back(entity);
}

Scene& Scene::addSystem(const System& system) {
    mSystems.push_back(system);
    return *this;
}

Scene& Scene::addFixedSystem(const System& system) {
    mFixedSystems.push_back(system);
    return *this;
}

Scene& Scene::addEventSystem(const EventSystem& system) {
    mEvtSystems.push_back(system);
    return *this;
}

void Scene::update() {
    for (const auto& system : mSystems) {
        system(mEntities);
    }
}

void Scene::tick() {
    for (const auto& system : mFixedSystems) {
        system(mEntities);
    }
}


void Scene::onEvent(const sf::Event& event) {
    for (const auto& system : mEvtSystems) {
        system(event, mEntities);
    }
}

void Scene::onMainLoopEnd() {
    for (const auto& entity : mRemovals){
        mEntities.erase(std::remove(mEntities.begin(), mEntities.end(), entity), mEntities.end());
        mRegistry.clear(entity.id());
    }
    mRemovals.clear();
}

std::vector<Entity> Scene::entities() const {
    return mEntities;
}

Entity findEntityByName(const std::vector<Entity> &entities, const std::string& name) {
    auto it = std::find_if(entities.begin(), entities.end(), 
        [name](Entity entity) {
            return entity.get<Tag>()->name == name;
        }
    );
    R_ASSERT(it != entities.end(), "no entity with name `" + name + "` found");
    return *it;
}

Entity findEntityById(const std::vector<Entity>& entities, EntityId id) {
    auto it = std::find_if(entities.begin(), entities.end(), 
        [id](Entity entity) {
            return entity.id() == id;
        }
    );
    R_ASSERT(it != entities.end(), "no entity with id `" + std::to_string(id) + "` found");
    return *it;
}

std::vector<Entity> findEntitiesByName(const std::vector<Entity> &entities, const std::string &name) {
    std::vector<Entity> filtered;
    std::copy_if(entities.begin(), entities.end(), std::back_inserter(filtered),
        [name](Entity entity) {
            return entity.get<Tag>()->name == name;
        }
    );
    return filtered;
}
