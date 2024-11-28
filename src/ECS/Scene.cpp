#include "Scene.hpp"
#include "Entity.hpp"
#include <Components/SpriteSystems.hpp>
#include <Components/Transform.hpp>

i32 Scene::sEntityCounter = 0;

Scene::Scene() {
    addSystem(spriteTransformSystem);
    addSystem(spriteAnimationSystem);
}

Scene::~Scene() = default;

Entity Scene::newEntity(const sf::Vector2f& initialPosition) {
    Entity newEntity (sEntityCounter++, *this);
    newEntity.add<Transform>(initialPosition, sf::Vector2f(1.f, 1.f), 0.f);
    mEntities.push_back(newEntity);
    return newEntity;
}

Scene& Scene::addSystem(const System& system) {
    mSystems.push_back(system);
    return *this;
}

void Scene::update() {
    for (const auto& system : mSystems) {
        system(mEntities);
    }
}

std::vector<Entity> Scene::getEntities() const {
    return mEntities;
}
