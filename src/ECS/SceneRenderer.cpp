#include "SceneRenderer.hpp"
#include "Entity.hpp"
#include <Components/Sprite.hpp>

SceneRenderer::SceneRenderer(Scene& scene, sf::RenderWindow& window) 
    : mScene(scene), mWindow(window) {}

void SceneRenderer::render() {
    renderSprites(mScene.getEntities());
}

void SceneRenderer::renderSprites(const std::vector<Entity>& entities) {
    for (const Entity& entity : entities) {
        if (!entity.has<Sprite>()) continue;

        auto* sprite = entity.get<Sprite>();
        mWindow.draw(sprite->sprite);
    }
}
