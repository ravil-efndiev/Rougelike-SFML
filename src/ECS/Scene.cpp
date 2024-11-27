#include "Scene.hpp"
#include "Entity.hpp"

template <class ...ComponentTypes>
std::vector<Entity> getMatchingEntities(const std::vector<Entity>& entities) {
    std::vector<Entity> matching;

    for (auto entity : entities) {
        if ((entity.has<ComponentTypes>() && ...)) {
            matching.push_back(entity);
        }
    }

    return matching;
}

i32 Scene::sEntityCounter = 0;

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
