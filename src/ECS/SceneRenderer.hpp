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
    void renderTilemaps(const std::vector<Entity>& entities);
    void renderDebug(const std::vector<Entity>& entities);
};
