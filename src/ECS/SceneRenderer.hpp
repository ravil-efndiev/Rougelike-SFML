#pragma once
#include "Scene.hpp"

class SceneRenderer {
public:
    SceneRenderer(Scene& scene, sf::RenderWindow& window);
    ~SceneRenderer() = default;

    void render();

private:
    Scene& mScene;
    sf::RenderWindow& mWindow;

private:
    void renderSprites(const std::vector<Entity>& entities);
    void renderVfx(const std::vector<Entity>& entities);
    void renderTilemaps(const std::vector<Entity>& entities, const std::vector<std::string>& layerNames);
    void renderDebug(const std::vector<Entity>& entities);
};
