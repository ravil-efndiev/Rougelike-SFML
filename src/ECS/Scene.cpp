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
    Entity newEntity (sEntityCounter++, *this);
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

Scene& Scene::addSystem(const System& system) {
    mSystems.push_back(system);
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

void Scene::onEvent(const sf::Event& event) {
    for (const auto& system : mEvtSystems) {
        system(event, mEntities);
    }
}


std::vector<Entity> Scene::getEntities() const {
    return mEntities;
}

Entity findEntityByName(const std::vector<Entity> &entities, const std::string& name) {
    auto it = std::find_if(entities.begin(), entities.end(), 
        [name](Entity entity) {
            return entity.get<Tag>()->name == name;
        }
    );
    if (it != entities.end()) {
        return *it;
    }
    throw std::out_of_range("no entity with name `" + name + "` found");
}
