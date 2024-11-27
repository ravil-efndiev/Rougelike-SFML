#include "Scene.hpp"
#include "Entity.hpp"
#include <Components/SpriteSystems.hpp>

i32 Scene::sEntityCounter = 0;

Scene::Scene() {
    addSystem(spriteTransformSystem);
}

Scene::~Scene() = default;

Entity Scene::newEntity() {
    Entity newEntity (sEntityCounter++, *this);
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
