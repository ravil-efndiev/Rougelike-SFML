#include "SceneRenderer.hpp"
#include "Entity.hpp"
#include <Components/Sprite.hpp>
#include <Components/Collider.hpp>
#include <Components/Tag.hpp>
#include <Gameplay/Tilemap.hpp>
#include <Gameplay/ColliderList.hpp>
#include <Gameplay/BloodEffect.hpp>

SceneRenderer::SceneRenderer(Scene& scene, sf::RenderWindow& window) 
    : mScene(scene), mWindow(window) {}

void SceneRenderer::render() {
    renderTilemaps(mScene.entities());
    renderSprites(mScene.entities());
    renderVfx(mScene.entities());
    renderDebug(mScene.entities());
}

void SceneRenderer::renderSprites(const std::vector<Entity>& entities) {
    for (const Entity& entity : entities) {
        if (!entity.has<Sprite>() || entity.has<BloodEffect>()) continue;

        auto* sprite = entity.get<Sprite>();
        R_ASSERT(sprite->texture, "sprite must have a texture initialized")

        mWindow.draw(sprite->sprite);
    }
}

void SceneRenderer::renderVfx(const std::vector<Entity>& entities) {
    for (const Entity& entity : entities) {
        if (!entity.has<BloodEffect>()) continue;

        auto* sprite = entity.get<Sprite>();
        auto* effect = entity.get<BloodEffect>();
        R_ASSERT(sprite->texture, "sprite must have a texture initialized")

        if (effect->active)
            mWindow.draw(sprite->sprite);
    }
}

void drawDebugRect(sf::RectangleShape& rect, sf::RenderWindow& window) {
    rect.setFillColor({0, 0, 0, 0});
    rect.setOutlineColor({0, 255, 0, 255});
    rect.setOutlineThickness(2);
    window.draw(rect);
}

void SceneRenderer::renderDebug(const std::vector<Entity>& entities) {
    for (const Entity& entity : entities) {
        if (entity.has<Collider>()) {
            auto* coll = entity.get<Collider>();
            if (coll->debugRender && coll->active) {
                sf::RectangleShape rect ({coll->bounds.width, coll->bounds.height});
                rect.setPosition(coll->bounds.left, coll->bounds.top);
                drawDebugRect(rect, mWindow);
            }
        }
        if (entity.has<ColliderList>()) {
            auto* collList = entity.get<ColliderList>();

            for (const Collider& coll : collList->colliders) {
                if (!coll.debugRender || !coll.active) continue;

                sf::RectangleShape rect ({coll.bounds.width, coll.bounds.height});
                rect.setPosition(coll.bounds.left, coll.bounds.top);
                drawDebugRect(rect, mWindow);
            }
        }
    }
}

void SceneRenderer::renderTilemaps(const std::vector<Entity>& entities) {
    for (const Entity& entity : entities) {
        if (!entity.has<Tilemap>()) continue;

        auto* tlm = entity.get<Tilemap>();

        for (auto& tile : tlm->tiles) {
            if (!tile.initialized()) continue;

            auto& sprite = tile.sprite();
            sprite.setPosition(tile.realPosition());
            mWindow.draw(sprite);
        }
    }
}
