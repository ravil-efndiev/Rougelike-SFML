#include "SceneRenderer.hpp"
#include "Entity.hpp"
#include <Components/Sprite.hpp>
#include <Components/Collider.hpp>
#include <Components/Tag.hpp>

SceneRenderer::SceneRenderer(Scene& scene, sf::RenderWindow& window) 
    : mScene(scene), mWindow(window) {}

void SceneRenderer::render() {
    renderSprites(mScene.getEntities());
    renderDebug(mScene.getEntities());
}

void SceneRenderer::renderSprites(const std::vector<Entity>& entities) {
    for (const Entity& entity : entities) {
        if (!entity.has<Sprite>()) continue;

        auto* sprite = entity.get<Sprite>();
        mWindow.draw(sprite->sprite);
    }
}

void SceneRenderer::renderDebug(const std::vector<Entity>& entities) {
    for (const Entity& entity : entities) {
        if (!entity.has<Collider>()) continue;

        auto* coll = entity.get<Collider>();
        if (coll->debugRender && coll->active) {
            sf::RectangleShape rect ({coll->bounds.width, coll->bounds.height});
            rect.setPosition(coll->bounds.left, coll->bounds.top);
            rect.setFillColor({0, 0, 0, 0});
            rect.setOutlineColor({0, 255, 0, 255});
            rect.setOutlineThickness(2);
            mWindow.draw(rect);
        }
    }
}
