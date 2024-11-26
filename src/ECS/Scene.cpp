#include "Scene.hpp"

i32 Scene::sEntityCounter = 0;

Scene::Scene() {

}

Entity Scene::newEntity() {
    Entity newEntity (sEntityCounter++);
    mEntities.push_back(newEntity);
    return newEntity;
}
